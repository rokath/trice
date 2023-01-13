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
        TRICE16( ID( 3037),"MSG: ✅… STOP  TriceDepth = %d, TriceDepthMax = %d + %4u of %d\n", triceDepth, TRICE_DATA_OFFSET, triceDepthMax-TRICE_DATA_OFFSET, TRICE_HALF_BUFFER_SIZE );
    } 
    #endif
    #if TRICE_MODE == TRICE_STREAM_BUFFER
        TRICE( ID( 4833), "MSG:triceFifoDepthMax = %d of max %d, triceStreamBufferDepthMax = %d of max %d\n", triceFifoDepthMax, TRICE_FIFO_ELEMENTS, triceStreamBufferDepthMax, TRICE_BUFFER_SIZE );
    #endif
}

//! TriceCheckSet writes out all types of trices with fixed values for testing
//! \details One trice has one subtrace, if param size max 2 bytes. 
//! Traces with more bytes as parameter consist of several subtraces.
void TriceCheckSet(int index) {
    char* A = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    char* s = "AAAAAAAAAAAA";
    float  x = (float)1089.6082763671875; // 0x44883377
    double y = 518.0547492508867; // 0x4080307020601050
    switch (index) {
           case __LINE__: 

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
  //break; case __LINE__: TRICE      ( id(0),    "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, inlined code behind, no stamp
  //break; case __LINE__: TRICE      ( Id(0),    "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, inlined code behind, 16-bit stamp
  //break; case __LINE__: TRICE      ( ID(0),    "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, inlined code behind, 32-bit stamp
    break; case __LINE__: TRICE      ( id(7009), "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, empty macro for patching, inlined code behind, no stamp
    break; case __LINE__: TRICE      ( Id(7009), "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, empty macro for patching, inlined code behind, 16-bit stamp
    break; case __LINE__: TRICE      ( ID(7009), "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, empty macro for patching, inlined code behind, 32-bit stamp

    // no stamp:
    break; case __LINE__: trice8     (           "msg:value=%d\n", -__LINE__ ); // user written code, empty macro for patching, no stamp
    break; case __LINE__: trice8M    ( 7009,     "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, calls internally fuction trice8fn_1, no stamp
    break; case __LINE__: trice8_1   (           "msg:value=%d\n", -__LINE__ ); // user written code, empty macro for patching
    break; case __LINE__: trice8M_1  ( 7009,     "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, calls internally fuction trice8fn_1
    break; case __LINE__: trice8fn_1 ( 7009,              (uint8_t)-__LINE__ ); // This is a direct function call, which works, but is not inteded for user code.

  //break; case __LINE__: TRICE8     (           "msg:value=%d\n", -__LINE__ ); // legacy with default stamp
  //break; case __LINE__: TRICE8_1   (           "msg:value=%d\n", -__LINE__ ); // legacy with default stamp
  //break; case __LINE__: TRICE8     ( id(0),    "msg:value=%d\n", -__LINE__ ); // user written code without stamp
  //break; case __LINE__: TRICE8_1   ( id(0),    "msg:value=%d\n", -__LINE__ ); // user written code without stamp
    break; case __LINE__: TRICE8     ( id(7009), "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, inlines code, no stamp
    break; case __LINE__: TRICE8_1   ( id(7009), "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, inlines code, no stamp
     
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
    break; case __LINE__: TRICE8     ( Id(7009), "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, inlines code, with 16-bit stamp
    break; case __LINE__: TRICE8_1   ( Id(7009), "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, inlines code, with 16-bit stamp
     
    // 32-bit stamp:
    break; case __LINE__: TRice8     (           "msg:value=%d\n", -__LINE__ ); // user written code, empty macro for patching, 32-bit stamp
    break; case __LINE__: TRice8M    ( 7009,     "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, calls internally fuction TRice8fn_1, 32-bit stamp
    break; case __LINE__: TRice8_1   (           "msg:value=%d\n", -__LINE__ ); // user written code, empty macro for patching
    break; case __LINE__: TRice8M_1  ( 7009,     "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, calls internally fuction TRice8fn_1
    break; case __LINE__: TRice8fn_1 ( 7009,              (uint8_t)-__LINE__ ); // This is a direct function call, which works, but not inteded for user code.

  //break; case __LINE__: TRICE8     (           "msg:value=%d\n", -__LINE__ ); // legacy default with default stamp
  //break; case __LINE__: TRICE8_1   (           "msg:value=%d\n", -__LINE__ ); // legacy default with default stamp
  //break; case __LINE__: TRICE8     ( ID(0),    "msg:value=%d\n", -__LINE__ ); // user written code with 32-bit stamp
  //break; case __LINE__: TRICE8_1   ( ID(0),    "msg:value=%d\n", -__LINE__ ); // user written code with 32-bit stamp
    break; case __LINE__: TRICE8     ( ID(7009), "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, inlines code, with 32-bit stamp
    break; case __LINE__: TRICE8_1   ( ID(7009), "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, inlines code, with 32-bit stamp
     
           
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
                           TRICE( ID( 4716), "msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e ); //lint !e666
                           TRICE( ID( 1570), "1/11 = %g\n", aFloat( 1.0/11 ) ); //lint !e666
                       }
    break; case __LINE__: TRICE( id( 7420), "FATAL:magenta+b:red\n" );
    break; case __LINE__: TRICE( id( 2244), "CRITICAL:red+i:default+h\n" );
    break; case __LINE__: TRICE( id( 1977), "EMERGENCY:red+i:blue\n" );
    break; case __LINE__: TRICE( id( 7846), "ERROR:11:red\n" );
    break; case __LINE__: TRICE( id( 2831), "WARNING:11+i:red\n" );
    break; case __LINE__: TRICE( id( 3150), "ATTENTION:11:green\n" );
    break; case __LINE__: TRICE( id( 1327), "INFO:cyan+b:default+h\n" );
    break; case __LINE__: TRICE( id( 3111), "DEBUG:130+i\n" );
    break; case __LINE__: TRICE( id( 3159), "TRACE:default+i:default+h\n" );
    break; case __LINE__: TRICE( id( 2039), "TIME:blue+i:blue+h\n" );
    break; case __LINE__: TRICE( id( 5589), "MESSAGE:green+h:black\n" );
    break; case __LINE__: TRICE( id( 7322), "READ:black+i:yellow+h\n" );
    break; case __LINE__: TRICE( id( 7707), "WRITE:black+u:yellow+h\n" );
    break; case __LINE__: TRICE( id( 6334), "RECEIVE:black+h:black\n" );
    break; case __LINE__: TRICE( id( 1556), "TRANSMIT:black:black+h\n" );
    break; case __LINE__: TRICE( id( 6822), "DIAG:yellow+i:default+h\n" );
    break; case __LINE__: TRICE( id( 6078), "INTERRUPT:magenta+i:default+h\n" );
    break; case __LINE__: TRICE( id( 7224), "SIGNAL:118+i\n" );
    break; case __LINE__: TRICE( id( 1931), "TEST:yellow+h:black\n" );
    break; case __LINE__: TRICE( id( 7227), "DEFAULT:off\n" );
    break; case __LINE__: TRICE( id( 4609), "NOTICE:blue:white+h\n" );
    break; case __LINE__: TRICE( id( 1296), "ALERT:magenta:magenta+h\n" );
    break; case __LINE__: TRICE( id( 5292), "ASSERT:yellow+i:blue\n" );
    break; case __LINE__: TRICE( id( 7304), "ALARM:red+i:white+h\n" );
    break; case __LINE__: TRICE( id( 5651), "CYCLE:blue+i:default+h\n" );
    break; case __LINE__: TRICE( id( 1800), "VERBOSE:blue:default\n" );
    break; case __LINE__: TRICE( id( 1322), "fatal:magenta+b:red\n" );
    break; case __LINE__: TRICE( id( 6227), "critical:red+i:default+h\n" );
    break; case __LINE__: TRICE( id( 6257), "emergency:red+i:blue\n" );
    break; case __LINE__: TRICE( id( 7793), "error:11:red\n" );
    break; case __LINE__: TRICE( id( 1052), "warning:11+i:red\n" );
    break; case __LINE__: TRICE( id( 2635), "attention:11:green\n" );
    break; case __LINE__: TRICE( id( 6972), "info:cyan+b:default+h\n" );
    break; case __LINE__: TRICE( id( 3148), "debug:130+i\n" );
    break; case __LINE__: TRICE( id( 6639), "trace:default+i:default+h\n" );
    break; case __LINE__: TRICE( id( 2521), "tme:blue+i:blue+h\n" );
    break; case __LINE__: TRICE( id( 5572), "message:green+h:black\n" );
    break; case __LINE__: TRICE( id( 4573), "read:black+i:yellow+h\n" );
    break; case __LINE__: TRICE( id( 1606), "write:black+u:yellow+h\n" );
    break; case __LINE__: TRICE( id( 2807), "receive:black+h:black\n" );
    break; case __LINE__: TRICE( id( 5762), "transmit:black:black+h\n" );
    break; case __LINE__: TRICE( id( 1929), "diag:yellow+i:default+h\n" );
    break; case __LINE__: TRICE( id( 2022), "interrupt:magenta+i:default+h\n" );
    break; case __LINE__: TRICE( id( 6837), "signal:118+i\n" );
    break; case __LINE__: TRICE( id( 4171), "test:yellow+h:black\n" );
    break; case __LINE__: TRICE( id( 4554), "default:off\n" );
    break; case __LINE__: TRICE( id( 5163), "notice:blue:white+h\n" );
    break; case __LINE__: TRICE( id( 3309), "allert:magenta:magenta+h\n" );
    break; case __LINE__: TRICE( id( 3897), "assert:yellow+i:blue\n" );
    break; case __LINE__: TRICE( id( 5118), "alarm:red+i:white+h\n" );
    break; case __LINE__: TRICE( id( 4798), "cycle:blue+i:default+h\n" );
    break; case __LINE__: TRICE( id( 3641), "verbose:blue:default\n" );
    break; case __LINE__: TRICE( id( 6183), "dbg:Hi!\n" );
    break; case __LINE__: TRICE( id( 2818), "dbg:\tHi!\n" );
    break; case __LINE__: TRICE( id( 6245), "dbg:\t\tHi!\n" );
    break; case __LINE__: TRICE( id( 7445), "dbg:\t\t\tHi!\n" );
    break; case __LINE__: TRICE( id( 5722), "dbg:\\tHi!\n" );
    //break; case __LINE__: TRICE( id( 7147), "dbg:\aHi!\n" ); // sound!
    //break; case __LINE__: TRICE( id( 3715), "dbg:\a\aHi!\n" ); // sound!
    //break; case __LINE__: TRICE( id( 2096), "dbg:\a\a\aHi!\n" ); // sound!
    break; case __LINE__: TRICE( id( 6987), "dbg:\\aHi!\n" );
    break; case __LINE__: {
                              unsigned len = strlen(s);
                              TRICE32( id( 7354), "dbg:len=%u:", len );
                              TRICE_S( id( 5129), "sig:TRICE_S=%s\n", s );
                              TRICE32( id( 1439), "dbg:len=%u:", len);
                              TRICE_N( id( 5182), "sig:TRICE_N=%s\n", s, len );

                              TRICE( id( 7260), "rd:TRICE line %E (%%E)\n", aFloat(-555555555.5555555555) );
                              TRICE( id( 3291), "rd:TRICE line %F (%%F)\n", aFloat(-555555555.5555555555) );
                              TRICE( id( 4343), "rd:TRICE line %G (%%G)\n", aFloat(-555555555.5555555555) );
                          }
    break; case __LINE__: TRICE32_1( id( 7679), "rd:TRICE32_1 line %u (%%u)\n", -1 );
    break; case __LINE__: TRICE32_1( id( 5550), "rd:TRICE32_1 line %b (%%b)\n", -1 );
    break; case __LINE__: TRICE32_1( id( 7867), "rd:TRICE32_1 line %o (%%o)\n", -1 );
    break; case __LINE__: TRICE32_1( id( 7875), "rd:TRICE32_1 line %O (%%O)\n", -1 );
    break; case __LINE__: TRICE32_1( id( 4680), "rd:TRICE32_1 line %X (%%X)\n", -1 );
    break; case __LINE__: TRICE32_1( id( 5219), "rd:TRICE32_1 line %x (%%x)\n", -1 );
    break; case __LINE__: TRICE32_1( id( 1169), "rd:TRICE32_1 line %d (%%d)\n", -1 );
    break; case __LINE__: TRICE64_1( id( 1058), "rd:TRICE64_1 line %u (%%u)\n", -1 );
    break; case __LINE__: TRICE64_1( id( 6341), "rd:TRICE64_1 line %b (%%b)\n", -1 );
    break; case __LINE__: TRICE64_1( id( 2590), "rd:TRICE64_1 line %o (%%o)\n", -1 );
    break; case __LINE__: TRICE64_1( id( 7612), "rd:TRICE64_1 line %O (%%O)\n", -1 );
    break; case __LINE__: TRICE64_1( id( 4929), "rd:TRICE64_1 line %X (%%C)\n", -1 );
    break; case __LINE__: TRICE64_1( id( 2778), "rd:TRICE64_1 line %x (%%x)\n", -1 );
    break; case __LINE__: TRICE64_1( id( 7508), "rd:TRICE64_1 line %d (%%d)\n", -1 );
    break; case __LINE__: TRICE64( id( 7618), "rd:TRICE64 line %E (%%E)\n", aDouble(-555555555.5555555555) );
    break; case __LINE__: TRICE64( id( 5474), "rd:TRICE64 line %F (%%F)\n", aDouble(-555555555.5555555555) );
    break; case __LINE__: TRICE64( id( 3164), "rd:TRICE64 line %G (%%G)\n", aDouble(-555555555.5555555555) );
    break; case __LINE__: TRICE( id( 5683), "sig:Some time measurements\n" );
    break; case __LINE__: TRICE( id( 2113), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
    break; case __LINE__: TRICE( id( 1076), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
    break; case __LINE__: TRICE( id( 2052), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
    break; case __LINE__: TRICE( id( 7509), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
    break; case __LINE__: TRICE64( id( 1524), "rd:TRICE64 %d, %d\n", 1, 2 );
    break; case __LINE__: TRICE( id( 7306), "tim:TRICE START time message\n");
    break; case __LINE__: TRICE( id( 2595), "tim:TRICE STOP time message\n");
    break; case __LINE__: TRICE( id( 4965), "tim:TRICE START time message\n");
    break; case __LINE__: TRICE( id( 7388), "tim:TRICE STOP time message\n");
    break; case __LINE__: TRICE( id( 4826), "tim:TRICE START time message\n");
    break; case __LINE__: TRICE64( id( 7624), "rd:TRICE64 %d, %d\n", 1, 2 );
    break; case __LINE__: TRICE( id( 1898), "tim:TRICE STOP time message\n");
    break; case __LINE__: TRICE( id( 6283), "tim:TRICE --------------------------------------------------\n");
    break; case __LINE__: TRICE( id( 5556), "tim:TRICE --------------------------------------------------\n");
    break; case __LINE__: TRICE64( id( 2746), "rd:TRICE64 %d, %d\n", 1, 2 );
    break; case __LINE__: TRICE( id( 2869), "tim:TRICE --------------------------------------------------\n");
    break; case __LINE__: TRICE( id( 3506), "tim:TRICE --------------------------------------------------\n");
    break; case __LINE__: TRICE64( id( 4889), "rd:TRICE64 %d, %d\n", 1, 2 );
    break; case __LINE__: TRICE( id( 3885),  "rd:TRICE32_1 line %d\n", __LINE__ );
    break; case __LINE__: TRICE64_1( id( 4144),  "rd:TRICE64_1 %d\n", __LINE__ );
    break; case __LINE__: TRICE32_2( id( 1603),  "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
    break; case __LINE__: TRICE64_2( id( 7163),  "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );

    break; case __LINE__: TRICE( id( 6182), "sig:TRICE with 1 to 12 values\n" );
    break; case __LINE__: TRICE( id( 4045),"rd:TRICE %d\n", -1 );
    break; case __LINE__: TRICE( id( 5585),"rd:TRICE %d, %d\n", -1, -2 );
    break; case __LINE__: TRICE( id( 5114),"rd:TRICE %d, %d, %d\n", -1, -2, -3 );
    break; case __LINE__: TRICE( id( 6099),"rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );
    break; case __LINE__: TRICE( id( 2493),"rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
    break; case __LINE__: TRICE( id( 3618),"rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
    break; case __LINE__: TRICE( id( 7990),"rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
    break; case __LINE__: TRICE( id( 3339),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
    break; case __LINE__: TRICE( id( 2232),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
    break; case __LINE__: TRICE( id( 6850),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
    break; case __LINE__: TRICE( id( 6090),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
    break; case __LINE__: TRICE( id( 5495),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE( id( 4369), "sig:TRICE_n with 1 to 12 values\n" );
    break; case __LINE__: TRICE_1( id( 6881),   "rd:TRICE_1 %d\n", 1 );
    break; case __LINE__: TRICE_2( id( 6054),   "rd:TRICE_2 %d, %d\n", 1, 2 );
    break; case __LINE__: TRICE_3( id( 7515),   "rd:TRICE_3 %d, %d, %d\n", 1, 2, 3 );
    break; case __LINE__: TRICE_4( id( 4409),   "rd:TRICE_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
    break; case __LINE__: TRICE_5( id( 6543),   "rd:TRICE_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
    break; case __LINE__: TRICE_6( id( 5706),   "rd:TRICE_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
    break; case __LINE__: TRICE_7( id( 3749),   "rd:TRICE_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
    break; case __LINE__: TRICE_8( id( 1187),   "rd:TRICE_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
    break; case __LINE__: TRICE_9( id( 5157),   "rd:TRICE_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
    break; case __LINE__: TRICE_10( id( 4191),  "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    break; case __LINE__: TRICE_11( id( 5030),  "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
    break; case __LINE__: TRICE_12( id( 1708),  "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );				
  //break; case __LINE__: TRICE( id( 2228), "sig:TRICE8 with 1 to 12 values\n" );
  //break; case __LINE__: TRICE8( id( 4777),"rd:TRICE8 %d\n", -1 );
  //break; case __LINE__: TRICE8( id( 6821),"rd:TRICE8 %d, %d\n", -1, -2 );
  //break; case __LINE__: TRICE8( id( 2030),"rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE8( id( 2955),"rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE8( id( 6626),"rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE8( id( 4356),"rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE8( id( 4819),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE8( id( 3598),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE8( id( 1273),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE8( id( 5514),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
  //break; case __LINE__: TRICE8( id( 6823),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE8( id( 1093),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE( id( 6889), "sig:TRICE8_n with 1 to 12 values\n" );
  //break; case __LINE__: TRICE8_1( id( 4793),   "rd:TRICE8_1 %d\n", 1 );
  //break; case __LINE__: TRICE8_2( id( 1120),   "rd:TRICE8_2 %d, %d\n", 1, 2 );
  //break; case __LINE__: TRICE8_3( id( 2618),   "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
  //break; case __LINE__: TRICE8_4( id( 3465),   "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
  //break; case __LINE__: TRICE8_5( id( 5134),   "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
  //break; case __LINE__: TRICE8_6( id( 5101),   "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
  //break; case __LINE__: TRICE8_7( id( 3444),   "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
  //break; case __LINE__: TRICE8_8( id( 6625),   "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
  //break; case __LINE__: TRICE8_9( id( 4028),   "rd:TRICE8_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
  //break; case __LINE__: TRICE8_10( id( 5214),  "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  //break; case __LINE__: TRICE8_11( id( 3273),  "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
  //break; case __LINE__: TRICE8_12( id( 4859),  "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
  //break; case __LINE__: TRICE( id( 7429), "sig:TRICE16 with 1 to 12 values\n" );
  //break; case __LINE__: TRICE16( id( 3404),"rd:TRICE16 %d\n", -1 );
  //break; case __LINE__: TRICE16( id( 6706),"rd:TRICE16 %d, %d\n", -1, -2 );
  //break; case __LINE__: TRICE16( id( 3774),"rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE16( id( 4243),"rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE16( id( 6266),"rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE16( id( 6750),"rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE16( id( 6634),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE16( id( 1036),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE16( id( 2626),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE16( id( 7537),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
  //break; case __LINE__: TRICE16( id( 6097),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE16( id( 6891),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE( id( 6800), "sig:TRICE16_n with 1 to 12 values\n" );
  //break; case __LINE__: TRICE16_1( id( 2697),  "rd:TRICE16_1 %d\n", 1 );
  //break; case __LINE__: TRICE16_2( id( 4552),  "rd:TRICE16_2 %d, %d\n", 1, 2 );
  //break; case __LINE__: TRICE16_3( id( 5086),  "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
  //break; case __LINE__: TRICE16_4( id( 7226),  "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
  //break; case __LINE__: TRICE16_5( id( 4651),  "rd:TRICE16_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
  //break; case __LINE__: TRICE16_6( id( 2009),  "rd:TRICE16_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
  //break; case __LINE__: TRICE16_7( id( 1805),  "rd:TRICE16_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
  //break; case __LINE__: TRICE16_8( id( 4458),  "rd:TRICE16_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
  //break; case __LINE__: TRICE16_9( id( 4396),  "rd:TRICE16_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
  //break; case __LINE__: TRICE16_10( id( 3901), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  //break; case __LINE__: TRICE16_11( id( 1196), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
  //break; case __LINE__: TRICE16_12( id( 5622), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
  //break; case __LINE__: TRICE( id( 3923), "sig:TRICE16 with 1 to 12 pointer\n" );
  //break; case __LINE__: TRICE16( id( 2676),"rd:TRICE16 %p\n", -1 );
  //break; case __LINE__: TRICE16( id( 5677),"rd:TRICE16 %p, %p\n", -1, -2 );
  //break; case __LINE__: TRICE16( id( 1218),"rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
  //break; case __LINE__: TRICE16( id( 1822),"rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE16( id( 7576),"rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE16( id( 3413),"rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE16( id( 1460),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE16( id( 3125),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE16( id( 5870),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE16( id( 7559),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
  //break; case __LINE__: TRICE16( id( 2528),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE16( id( 3313),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE( id( 3303), "sig:TRICE16 with 1 to 12 hex\n" );
  //break; case __LINE__: TRICE16( id( 2057),"rd:TRICE16 %X\n", -1 );
  //break; case __LINE__: TRICE16( id( 2216),"rd:TRICE16 %X, %X\n", -1, -2 );
  //break; case __LINE__: TRICE16( id( 3850),"rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
  //break; case __LINE__: TRICE16( id( 2529),"rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE16( id( 7966),"rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE16( id( 2313),"rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE16( id( 2265),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE16( id( 7807),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE16( id( 6977),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE16( id( 6108),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
  //break; case __LINE__: TRICE16( id( 1700),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE16( id( 2700),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE( id( 3067), "sig:TRICE32 with 1 to 12 values\n" );
    break; case __LINE__: TRICE32( id( 5489),"rd:TRICE32 %d\n", -1 );
    break; case __LINE__: TRICE32( id( 5560),"rd:TRICE32 %d, %d\n", -1, -2 );
    break; case __LINE__: TRICE32( id( 3397),"rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );
    break; case __LINE__: TRICE32( id( 7650),"rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );
    break; case __LINE__: TRICE32( id( 7629),"rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
    break; case __LINE__: TRICE32( id( 5320),"rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
    break; case __LINE__: TRICE32( id( 7046),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
    break; case __LINE__: TRICE32( id( 6510),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
    break; case __LINE__: TRICE32( id( 1247),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
    break; case __LINE__: TRICE32( id( 6218),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
    break; case __LINE__: TRICE32( id( 6864),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
    break; case __LINE__: TRICE32( id( 3247),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE( id( 2394), "signal:TRICE32_n with 1 to 12 values\n" );
    break; case __LINE__: TRICE32_1( id( 6922),  "rd:TRICE32_1 %d\n", 1 );
    break; case __LINE__: TRICE32_2( id( 6409),  "rd:TRICE32_2 %d, %d\n", 1, 2 );
    break; case __LINE__: TRICE32_3( id( 4958),  "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
    break; case __LINE__: TRICE32_4( id( 3782),  "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
    break; case __LINE__: TRICE32_5( id( 6340),  "rd:TRICE32_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
    break; case __LINE__: TRICE32_6( id( 5869),  "rd:TRICE32_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
    break; case __LINE__: TRICE32_7( id( 3362),  "rd:TRICE32_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
    break; case __LINE__: TRICE32_8( id( 7378),  "rd:TRICE32_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
    break; case __LINE__: TRICE32_9( id( 3526),  "rd:TRICE32_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
    break; case __LINE__: TRICE32_10( id( 5859), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    break; case __LINE__: TRICE32_11( id( 4194), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
    break; case __LINE__: TRICE32_12( id( 7438), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
    break; case __LINE__: TRICE( id( 4843), "sig:TRICE64 with 1 to 12 values\n" );
  //break; case __LINE__: TRICE64( id( 7873),"rd:TRICE64 %d\n", -1 );
  //break; case __LINE__: TRICE64( id( 3768),"rd:TRICE64 %d, %d\n", -1, -2 );
  //break; case __LINE__: TRICE64( id( 1632),"rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE64( id( 7047),"rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE64( id( 6310),"rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE64( id( 1777),"rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE64( id( 2434),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE64( id( 1761),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE64( id( 2565),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE64( id( 3696),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
  //break; case __LINE__: TRICE64( id( 4096),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE64( id( 1306),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE( id( 4493), "sig:TRICE64_n with 1 to 12 values\n" );
  //break; case __LINE__: TRICE64_1( id( 2845),  "rd:TRICE64_1 %d\n", 1 );
  //break; case __LINE__: TRICE64_2( id( 7544),  "rd:TRICE64_2 %d, %d\n", 1, 2 );
  //break; case __LINE__: TRICE64_2( id( 4551),  "rd:TRICE64_2 %x, %x\n", 0x1122334455667788, 0x0102030405060708 );
  //break; case __LINE__: TRICE64_3( id( 6955),  "rd:TRICE64_3 %d, %d, %d\n", 1, 2, 3 );
  //break; case __LINE__: TRICE64_4( id( 2205),  "rd:TRICE64_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
  //break; case __LINE__: TRICE64_5( id( 7969),  "rd:TRICE64_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
  //break; case __LINE__: TRICE64_6( id( 6618),  "rd:TRICE64_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
  //break; case __LINE__: TRICE64_7( id( 6851),  "rd:TRICE64_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
  //break; case __LINE__: TRICE64_8( id( 5725),  "rd:TRICE64_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
  //break; case __LINE__: TRICE64_9( id( 5735),  "rd:TRICE64_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
  //break; case __LINE__: TRICE64_10( id( 1417), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  //break; case __LINE__: TRICE64_11( id( 3679), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
  //break; case __LINE__: TRICE64_12( id( 4982), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
  //break; case __LINE__: TRICE( id( 3564), "sig:Colors and ticks\n" );
  //break; case __LINE__: TRICE( id( 6565), "--------------------------------------------------\n");
  //break; case __LINE__: TRICE( id( 7666), "--------------------------------------------------\n");
  //break; case __LINE__: TRICE( id( 2906), "dbg:12345 as 16bit is %016b\n", 12345);
  //break; case __LINE__: TRICE( id( 2121), "--------------------------------------------------\n");
  //break; case __LINE__: TRICE( id( 5658), "sig:This ASSERT error is just a demo and no real error:\n");
  //break; case __LINE__: TRICE( id( 3703), "--------------------------------------------------\n");
  //break; case __LINE__: TRICE( id( 1516), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( id( 4591), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( id( 3908), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( id( 7346), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( id( 1316), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( id( 5543), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( id( 4586), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( id( 3455), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( id( 7021), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( id( 5773), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( id( 3047), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( id( 2459), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( id( 4444), "time:TRICE32_1  message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( id( 3487), "sig:Various ranges and formats\n" );
  //break; case __LINE__: TRICE8( id( 5316), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( id( 6631), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( id( 4454), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( id( 6179), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( id( 7208), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( id( 4288), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( id( 1486), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( id( 3425), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( id( 4601), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE16(Id( 5900), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
  //break; case __LINE__: TRICE16(Id( 5248), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
  //break; case __LINE__: TRICE16(Id( 3281), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);
  //break; case __LINE__: TRICE32(Id( 2836), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
  //break; case __LINE__: TRICE32(Id( 2810), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
  //break; case __LINE__: TRICE32(Id( 4886), "att:TRICE32 %40b\n", 0xAAAAAAAA);
  //break; case __LINE__: TRICE64(Id( 6724), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
  //break; case __LINE__: TRICE64(Id( 3611), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
  //break; case __LINE__: TRICE64(Id( 1221), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
  //break; case __LINE__: TRICE( id( 1655), "sig:Legacy TRICE8\n" );
  //break; case __LINE__: TRICE8_1(Id( 7808), "tst:TRICE8_1 %02x\n", 0xA1);
  //break; case __LINE__: TRICE8_2(Id( 2805), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
  //break; case __LINE__: TRICE8_3(Id( 4623), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
  //break; case __LINE__: TRICE8_4(Id( 3418), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
  //break; case __LINE__: TRICE8_5(Id( 2358), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
  //break; case __LINE__: TRICE8_6(Id( 7880), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
  //break; case __LINE__: TRICE8_7(Id( 3839), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
  //break; case __LINE__: TRICE8_8(Id( 2938), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
  //break; case __LINE__: TRICE8_1(Id( 4327), "tst:TRICE8_1 %d\n",                      -1);
  //break; case __LINE__: TRICE8_2(Id( 2787), "tst:TRICE8_2 %d %d\n",                   -1, -2);
  //break; case __LINE__: TRICE8_3(Id( 3021), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
  //break; case __LINE__: TRICE8_4(Id( 3142), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
  //break; case __LINE__: TRICE8_5(Id( 1509), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
  //break; case __LINE__: TRICE8_6(Id( 4666), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
  //break; case __LINE__: TRICE8_7(Id( 2524), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE8_8(Id( 3933), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
  //break; case __LINE__: TRICE8_1(Id( 2803), "tst:TRICE8_1 %d\n", 1);
  //break; case __LINE__: TRICE8_2(Id( 1765), "tst:TRICE8_2 %d %d\n", 1, 2);
  //break; case __LINE__: TRICE8_3(Id( 4080), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
  //break; case __LINE__: TRICE8_4(Id( 3319), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
  //break; case __LINE__: TRICE8_5(Id( 2498), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
  //break; case __LINE__: TRICE8_6(Id( 1792), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
  //break; case __LINE__: TRICE8_7(Id( 4497), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
  //break; case __LINE__: TRICE8_8(Id( 1030), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
  //break; case __LINE__: TRICE8_1(Id( 7491), "tst:TRICE8_1 %u\n", 201);
  //break; case __LINE__: TRICE8_2(Id( 2033), "tst:TRICE8_2 %u %u\n", 201, 202);
  //break; case __LINE__: TRICE8_3(Id( 4167), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
  //break; case __LINE__: TRICE8_4(Id( 3027), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
  //break; case __LINE__: TRICE8_5(Id( 3556), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
  //break; case __LINE__: TRICE8_6(Id( 1602), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
  //break; case __LINE__: TRICE8_7(Id( 7037), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
  //break; case __LINE__: TRICE8_8(Id( 5745), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
  //break; case __LINE__: TRICE8_8(Id( 5500), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);
    break; case __LINE__: TRICE( id( 7394), "sig:Legacy TRICE16-64\n" );
    break; case __LINE__: TRICE16_1(Id( 2902), "tst:TRICE16_1 %d\n", -111);
    break; case __LINE__: TRICE16_2(Id( 3091), "tst:TRICE16_2 %d %d\n", -111, -222);
    break; case __LINE__: TRICE16_3(Id( 7510), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
    break; case __LINE__: TRICE16_4(Id( 4728), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
    break; case __LINE__: TRICE32_1(Id( 1335), "tst:TRICE32_1 %08x\n", 0x0123cafe);
    break; case __LINE__: TRICE32_1(Id( 1727), "tst:TRICE32_1 %d\n", -111);
    break; case __LINE__: TRICE32_2(Id( 4596), "tst:TRICE32_2 %x %x\n", -111, -222);
    break; case __LINE__: TRICE32_2(Id( 4577), "tst:TRICE32_2 %d %d\n", -111, -222);
    break; case __LINE__: TRICE32_3(Id( 3456), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
    break; case __LINE__: TRICE32_3(Id( 5312), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
    break; case __LINE__: TRICE32_4(Id( 6859), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
    break; case __LINE__: TRICE32_4(Id( 5439), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
    break; case __LINE__: TRICE64_1(Id( 1165), "tst:TRICE64_1 %d\n", -111);
    break; case __LINE__: TRICE64_2(Id( 5308), "tst:TRICE64_2 %d %d\n", -111, -222);
    break; case __LINE__: TRICE16_1(Id( 2982), "tst:TRICE16_1 %u\n", 60001);
    break; case __LINE__: TRICE16_2(Id( 4308), "tst:TRICE16_2 %u %u\n", 60001, 60002);
    break; case __LINE__: TRICE16_3(Id( 7606), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
    break; case __LINE__: TRICE16_4(Id( 1786), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
    break; case __LINE__: TRICE32_1(Id( 2495), "tst:TRICE32_1 %u\n", 4000000001);
    break; case __LINE__: TRICE32_2(Id( 5082), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
    break; case __LINE__: TRICE32_3(Id( 3841), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
    break; case __LINE__: TRICE32_4(Id( 7625), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
    break; case __LINE__: TRICE64_1(Id( 3903), "tst:TRICE64_1 %x\n", -1);
    break; case __LINE__: TRICE64_2(Id( 4882), "tst:TRICE64_2 %x %x\n", -1, -2);
    break; case __LINE__: TRICE64_1(Id( 3959), "tst:TRICE64_1 %u\n", -1);
    break; case __LINE__: TRICE64_2(Id( 4034), "tst:TRICE64_2 %u %u\n", -1, -2);
  //break; case __LINE__: TRICE( id( 6005), "sig:colored single letters and several TRICE macros in one line\n" );
  //break; case __LINE__: TRICE(Id( 4159), "e:A");
  //break; case __LINE__: TRICE(Id( 5090), "w:B");
  //break; case __LINE__: TRICE(Id( 4048), "a:c");
  //break; case __LINE__: TRICE(Id( 1790), "wr:d");
  //break; case __LINE__: TRICE(Id( 7275), "rd:e\n");
  //break; case __LINE__: TRICE(Id( 1264), "diag:f");
  //break; case __LINE__: TRICE(Id( 7829), "d:G");
  //break; case __LINE__: TRICE(Id( 1744), "t:H");
  //break; case __LINE__: TRICE(Id( 4116), "time:i");
  //break; case __LINE__: TRICE(Id( 1555), "message:J");
  //break; case __LINE__: TRICE(Id( 6014), "dbg:k\n");
  //break; case __LINE__: TRICE(Id( 6472), "1");
  //break; case __LINE__: TRICE(Id( 5605), "2");
  //break; case __LINE__: TRICE(Id( 5268), "3");
  //break; case __LINE__: TRICE(Id( 4445), "4");
  //break; case __LINE__: TRICE(Id( 4132), "e:7");
  //break; case __LINE__: TRICE(Id( 3029), "m:12");
  //break; case __LINE__: TRICE(Id( 1893), "m:123\n");
  //break; case __LINE__: TRICE(Id( 1375), "e:A");TRICE(Id( 2917), "w:B");TRICE(Id( 5877), "a:c");
  //break; case __LINE__: TRICE(Id( 4632), "wr:d");TRICE(Id( 7539), "rd:e\n");//TRICE(Id( 2008), "diag:f");
/*
    break; case __LINE__: TRICE( id( 5420), "sig:Runtime generated strings\n" );
    break; case __LINE__: {
                              s = "AAAAAAAAAAAA";
                              TRICE32( id( 7741), "dbg:len=%u:", strlen(s) );
                              TRICE_S( id( 5176), "sig:%s\n", s );
                          }
    break; case __LINE__: {
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
                          }
    break; case __LINE__: TRICE32( id( 7063), "dbg:len=%u:", strlen(s) );
    break; case __LINE__: TRICE_S( id( 7637), "sig:%s\n", s );
    break; case __LINE__: TRICE( id( 3056), "sig:Runtime generated strings\n" );
    break; case __LINE__: {
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
                          }
    break; case __LINE__: TRICE32( id( 3009), "dbg:len=%u: ", strlen(s) );
    break; case __LINE__: TRICE_S( id( 4470), "sig:%s\n", s );
    break; case __LINE__: TRICE( id( 1014), "sig:Runtime generated strings\n" );
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
    break; case __LINE__: TRICE32( id( 3687), "dbg:len=%u: ", strlen(s) );
    break; case __LINE__: TRICE_S( id( 7719), "sig:%s\n", s );
    break; case __LINE__: TRICE( id( 4995), "sig:Runtime generated strings\n" );
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
    break; case __LINE__: TRICE32( id( 3619), "dbg:len=%u: ", strlen(s) );
    break; case __LINE__: TRICE_S( id( 1235), "sig:%s\n", s );
    break; case __LINE__: { // https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/
                              int i = 15;
                              TRICE( id( 2639), "sig:Integer (indent, base, sign)\n" );
                              TRICE( id( 3430), "rd: 15 	%d 	Base 10\n", i );
                              TRICE( id( 7801), "rd: +15 	%+d 	Always show sign\n", i );
                              TRICE( id( 5696), "rd:   15 	%4d 	Pad with spaces (width 4, right justified)\n", i );
                              TRICE( id( 3775), "rd: 15   	%-4d 	Pad with spaces (width 4, left justified)\n", i );
                              TRICE( id( 3043), "rd: 0015 	%04d 	Pad with zeroes (width 4)\n", i );
                              TRICE( id( 6229), "rd: 1111 	%b 	Base 2\n", i );
                              TRICE( id( 3192), "rd: 17 	%o 	Base 8\n", i );
                              TRICE( id( 2570), "rd: f 	%x 	Base 16, lowercase\n", i );
                              TRICE( id( 1470), "rd: F 	%X 	Base 16, uppercase\n", i );
                              TRICE( id( 1873), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", i );
                          }

    break; case __LINE__: {
                              char a = 'A';
                              TRICE( id( 6720), "sig:Character (quoted, Unicode)\n" );
                              TRICE( id( 7669), "rd: A 	%c 	Character\n", a );
                              TRICE( id( 2970), "rd: 'A' 	%q 	Quoted character\n", a );
                              TRICE( id( 6044), "rd: U+0041 	%U 	Unicode\n", a );// todo
                              TRICE( id( 1964), "rd: U+0041 'A' 	%#U 	Unicode with character\n", a );// todo
                              TRICE( id( 2567), "sig:Boolean (true/false)\n" );
                              TRICE( id( 7736), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 );
                              TRICE( id( 4795), "sig:Pointer (hex)\n" );
                          }

    break; case __LINE__: TRICE8( id( 7074), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );
    break; case __LINE__: TRICE16( id( 4532), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );
    break; case __LINE__: TRICE32( id( 1536), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );
    break; case __LINE__: TRICE64( id( 1507), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 );
    break; case __LINE__: TRICE( id( 1717), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);
    break; case __LINE__: {
                                float f = (float)123.456; 
                                TRICE( id( 4115), "sig:Float (indent, precision, scientific notation)\n" );
                                TRICE( id( 3554), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(f) );
                                TRICE( id( 2554), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(f) );
                                TRICE( id( 7773), "rd: 123.46	 		%.2f		%%.2f Default width, precision 2\n", aFloat(f) );
                                TRICE( id( 3790), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(f) );
                                TRICE( id( 3340), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(f) );
                          }
    break; case __LINE__: {
                                double d = 123.456;
                                TRICE( id( 7188), "sig:Double (indent, precision, scientific notation)\n" );
                                TRICE64( id( 7792), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(d) );
                                TRICE64( id( 6452), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(d) );
                                TRICE64( id( 3920), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(d) );
                                TRICE64( id( 4504), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(d) );
                                TRICE64( id( 2968), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(d) );
                          }
    break; case __LINE__: {
                              s = "café";
                              TRICE( id( 1972), "sig:String or byte slice (quote, indent, hex)\n" );
                              TRICE_S( id( 4139), "rd: café 			%s		Plain string\n", s );
                              TRICE_S( id( 6473), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );
                              TRICE_S( id( 7906), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );
                              TRICE_S( id( 3777), "rd: \"café\" 		%q	 	Quoted string\n", s );
                              TRICE_S( id( 1033), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );
                              TRICE_S( id( 4405), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );
                          }
*/
  //break; case __LINE__: TRICE( id( 1630), "sig:TRICE8 with variable param count 1 to 12\n" );
  //break; case __LINE__: TRICE8(Id( 4024), "tst:TRICE8 %d\n", -1 );
  //break; case __LINE__: TRICE8(Id( 6003), "tst:TRICE8 %d %d\n", -1, -2 );
  //break; case __LINE__: TRICE8(Id( 1360), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE8(Id( 7038), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE8(Id( 7053), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE8(Id( 3060), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE8(Id( 1308), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
  //break; case __LINE__: TRICE8(Id( 1363), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE8(Id( 5413), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE8(Id( 2100), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
  //break; case __LINE__: TRICE8(Id( 1424), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE8(Id( 6831), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE8(Id( 6042), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
  //break; case __LINE__: TRICE8_1 (Id( 1287), "tst:TRICE8_1  %d\n", -1 );
  //break; case __LINE__: TRICE8_2 (Id( 5600), "tst:TRICE8_2  %d %d\n", -1, -2 );
  //break; case __LINE__: TRICE8_3 (Id( 3422), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE8_4 (Id( 3603), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE8_5 (Id( 3630), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE8_6 (Id( 4221), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE8_7 (Id( 1293), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
  //break; case __LINE__: TRICE8_8 (Id( 7949), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE8_9 (Id( 5405), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE8_10(Id( 3609), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
  //break; case __LINE__: TRICE8_11(Id( 4328), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE8_12(Id( 6506), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE8  (Id( 4744), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
  //break; case __LINE__: TRICE8_9(Id( 3353), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
  //break; case __LINE__: TRICE( id( 6236), "sig:TRICE16 with variable param count 1 to 12\n" );
  //break; case __LINE__: TRICE16(Id( 3808), "tst:TRICE16 %d\n", -1 );
  //break; case __LINE__: TRICE16(Id( 5739), "tst:TRICE16 %d %d\n", -1, -2 );
  //break; case __LINE__: TRICE16(Id( 7710), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE16(Id( 3864), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE16(Id( 6936), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE16(Id( 4792), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE16(Id( 2019), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
  //break; case __LINE__: TRICE16(Id( 5896), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE16(Id( 6585), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE16(Id( 5295), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
  //break; case __LINE__: TRICE16(Id( 2157), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE16(Id( 6057), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE16(Id( 3417), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
  //break; case __LINE__: TRICE16_1 (Id( 7036), "tst:TRICE16_1  %d\n", -1 );
  //break; case __LINE__: TRICE16_2 (Id( 4296), "tst:TRICE16_2  %d %d\n", -1, -2 );
  //break; case __LINE__: TRICE16_3 (Id( 6391), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE16_4 (Id( 1661), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE16_5 (Id( 3800), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE16_6 (Id( 1517), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE16_7 (Id( 1963), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
  //break; case __LINE__: TRICE16_8 (Id( 7621), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE16_9 (Id( 7688), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE16_10(Id( 3950), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
  //break; case __LINE__: TRICE16_11(Id( 5743), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE16_12(Id( 1814), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE16_9(Id( 1374), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
    break; case __LINE__: TRICE( id( 7422), "sig:TRICE32 with variable param count 1 to 12\n" );
    break; case __LINE__: TRICE32(Id( 2468), "tst:TRICE32 %d\n", -1 );
    break; case __LINE__: TRICE32(Id( 3673), "tst:TRICE32 %d %d\n", -1, -2 );
    break; case __LINE__: TRICE32(Id( 6899), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
    break; case __LINE__: TRICE32(Id( 7262), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
    break; case __LINE__: TRICE32(Id( 3178), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
    break; case __LINE__: TRICE32(Id( 6903), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
    break; case __LINE__: TRICE32(Id( 5657), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
    break; case __LINE__: TRICE32(Id( 3812), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
    break; case __LINE__: TRICE32(Id( 4378), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
    break; case __LINE__: TRICE32(Id( 2074), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
    break; case __LINE__: TRICE32(Id( 1116), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
    break; case __LINE__: TRICE32(Id( 7701), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE32(Id( 2108), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
    break; case __LINE__: TRICE32_1 (Id( 7894), "tst:TRICE32_1  %d\n", -1 );
    break; case __LINE__: TRICE32_2 (Id( 5612), "tst:TRICE32_2  %d %d\n", -1, -2 );
    break; case __LINE__: TRICE32_3 (Id( 2072), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
    break; case __LINE__: TRICE32_4 (Id( 1499), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
    break; case __LINE__: TRICE32_5 (Id( 4587), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
    break; case __LINE__: TRICE32_6 (Id( 5830), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
    break; case __LINE__: TRICE32_7 (Id( 7732), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
    break; case __LINE__: TRICE32_8 (Id( 4772), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
    break; case __LINE__: TRICE32_9 (Id( 7577), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
    break; case __LINE__: TRICE32_10(Id( 2414), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
    break; case __LINE__: TRICE32_11(Id( 4996), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
    break; case __LINE__: TRICE32_12(Id( 7143), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE32   (Id( 5141), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
    break; case __LINE__: TRICE32_12(Id( 4629), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
    break; case __LINE__: TRICE( id( 6289), "sig:TRICE64 with variable param count 1 to 12\n" );
    break; case __LINE__: TRICE64(Id( 6544), "tst:TRICE64 %d\n", -1 );
    break; case __LINE__: TRICE64(Id( 1969), "tst:TRICE64 %d %d\n", -1, -2 );
    break; case __LINE__: TRICE64(Id( 7907), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
    break; case __LINE__: TRICE64(Id( 7692), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
    break; case __LINE__: TRICE64(Id( 4822), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
    break; case __LINE__: TRICE64(Id( 6175), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
    break; case __LINE__: TRICE64(Id( 7713), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
    break; case __LINE__: TRICE64(Id( 7466), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
    break; case __LINE__: TRICE64(Id( 2004), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
    break; case __LINE__: TRICE64(Id( 7210), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
    break; case __LINE__: TRICE64(Id( 1853), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
    break; case __LINE__: TRICE64(Id( 7598), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE64(Id( 3301), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
    break; case __LINE__: TRICE64_1 (Id( 1177), "tst:TRICE64_1  %d\n", -1 );
    break; case __LINE__: TRICE64_2 (Id( 1711), "tst:TRICE64_2  %d %d\n", -1, -2 );
    break; case __LINE__: TRICE64_3 (Id( 5366), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
    break; case __LINE__: TRICE64_4 (Id( 5684), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
    break; case __LINE__: TRICE64_5 (Id( 6946), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
    break; case __LINE__: TRICE64_6 (Id( 3996), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
    break; case __LINE__: TRICE64_7 (Id( 3656), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
    break; case __LINE__: TRICE64_8 (Id( 4247), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
    break; case __LINE__: TRICE64_9 (Id( 6178), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
    break; case __LINE__: TRICE64_10(Id( 6036), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
    break; case __LINE__: TRICE64_11(Id( 7443), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
    break; case __LINE__: TRICE64_12(Id( 2992), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE64_12(Id( 2967), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
    break; case __LINE__: TRICE( id( 7520), "att:positive and negative float in format variants\n");
    break; case __LINE__: TRICE( id( 4605), "rd:TRICE float %f (%%f)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 3606), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 7782), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 6833), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 6213), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 4432), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 2812), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 4013), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 1538), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 1983), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 7886), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 3683), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 4633), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 4261), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 6915), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 3701), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 2996), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 2370), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 5078), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 6084), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 7483), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 4069), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 3925), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 5119), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 5261), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 6172), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 5851), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 1586), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 6313), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 7112), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 3392), "att:positive float & double in variants\n");
    break; case __LINE__: TRICE32_1( id( 1311), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
    break; case __LINE__: TRICE32_1( id( 2784), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
    break; case __LINE__: TRICE32_1( id( 5212), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
    break; case __LINE__: TRICE32_1( id( 4686), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
    break; case __LINE__: TRICE32_1( id( 7390), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
    break; case __LINE__: TRICE32_1( id( 2156), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
    break; case __LINE__: TRICE32( id( 2398), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
    break; case __LINE__: TRICE32( id( 5737), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
    break; case __LINE__: TRICE32( id( 7337), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
    break; case __LINE__: TRICE32( id( 7454), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
    break; case __LINE__: TRICE32( id( 6132), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
    break; case __LINE__: TRICE32( id( 3584), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 7902), "rd:TRICE float %e (%%e)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 6447), "rd:TRICE float %f (%%f)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 4186), "rd:TRICE float %g (%%g)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 1784), "rd:TRICE float %E (%%E)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 1151), "rd:TRICE float %F (%%F)\n", aFloat(x) );
    break; case __LINE__: TRICE( id( 7325), "rd:TRICE float %G (%%G)\n", aFloat(x) );
    break; case __LINE__: TRICE64( id( 7423), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( id( 3911), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( id( 2683), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( id( 3795), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( id( 2309), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( id( 3402), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64_1( id( 2246), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64_1( id( 3502), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64_1( id( 3522), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64_1( id( 5373), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64_1( id( 6033), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64_1( id( 3952), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE( id( 6569), "att:negative float & double\n" );
    break; case __LINE__: TRICE( id( 3801), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 6756), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 4536), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 1770), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 5290), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
    break; case __LINE__: TRICE( id( 1890), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
    break; case __LINE__: TRICE64( id( 5723), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
    break; case __LINE__: TRICE64( id( 7252), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
    break; case __LINE__: TRICE64( id( 4535), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
    break; case __LINE__: TRICE64( id( 5874), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
    break; case __LINE__: TRICE64( id( 1907), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
    break; case __LINE__: TRICE64( id( 5738), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );
    break; case __LINE__: TRICE( id( 4366), "att:formatted float & double\n" );
    break; case __LINE__: TRICE32( id( 2658), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
    break; case __LINE__: TRICE32( id( 4865), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
    break; case __LINE__: TRICE32( id( 7560), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
    break; case __LINE__: TRICE32( id( 5902), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
    break; case __LINE__: TRICE32( id( 1164), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
    break; case __LINE__: TRICE32( id( 4932), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
    break; case __LINE__: TRICE64( id( 6113), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( id( 5603), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( id( 6598), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( id( 3097), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( id( 2154), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( id( 4949), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE( id( 7350), "att:mixed int & float & double & bits\n" );
    break; case __LINE__: TRICE32( id( 4362), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(x), aFloat(x), aFloat(x), aFloat(x) );
    break; case __LINE__: TRICE32( id( 7353), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(x), aFloat(x), aFloat(x), aFloat(x) );
    break; case __LINE__: TRICE32( id( 2105), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              x,  aFloat(x), aFloat(x), aFloat(x) );
    break; case __LINE__: TRICE32( id( 1841), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(-x), aFloat(-x), aFloat(-x), aFloat(-x) );
    break; case __LINE__: TRICE32( id( 7146), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(-x), aFloat(-x), aFloat(-x), aFloat(-x) );
    break; case __LINE__: TRICE32( id( 7026), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              -x,  aFloat(-x), aFloat(-x), aFloat(-x) );
    break; case __LINE__: TRICE64( id( 5593), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(y), aDouble(y), aDouble(y), aDouble(y) );
    break; case __LINE__: TRICE64( id( 4835), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(y), aDouble(y), aDouble(y), aDouble(y) );
    break; case __LINE__: TRICE64( id( 3987), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               y,  aDouble(y), aDouble(y), aDouble(y) );
    break; case __LINE__: TRICE64( id( 5698), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) );
    break; case __LINE__: TRICE64( id( 4801), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) );
    break; case __LINE__: TRICE64( id( 3306), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               -y,  aDouble(-y), aDouble(-y), aDouble(-y) );
  //break; case __LINE__: TRICE( id( 6124), "att:Various single arguments\n" );
  //break; case __LINE__: TRICE8( id( 1857), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
  //break; case __LINE__: TRICE8( id( 1495), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
  //break; case __LINE__: TRICE8( id( 4893), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
  //break; case __LINE__: TRICE8( id( 4235), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
  //break; case __LINE__: TRICE8( id( 7395), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
  //break; case __LINE__: TRICE8( id( 7189), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
  //break; case __LINE__: TRICE8( id( 6875), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
  //break; case __LINE__: TRICE8( id( 7752), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
  //break; case __LINE__: TRICE8( id( 1294), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
  //break; case __LINE__: TRICE16_1( id( 1220), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
  //break; case __LINE__: TRICE16_1( id( 5802), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
  //break; case __LINE__: TRICE16_1( id( 6024), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
  //break; case __LINE__: TRICE16_1( id( 4509), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
  //break; case __LINE__: TRICE16_1( id( 4277), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
  //break; case __LINE__: TRICE16_1( id( 2415), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
  //break; case __LINE__: TRICE16_1( id( 5288), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
    break; case __LINE__: TRICE( id( 1057), "rd:TRICE line %t (%%t -1)\n", 0 );
    break; case __LINE__: TRICE( id( 5876), "rd:TRICE line %t (%%t -1)\n", 2 );
    break; case __LINE__: TRICE( id( 6159), "rd:TRICE line %u (%%u)\n", -1 );
    break; case __LINE__: TRICE( id( 4896), "rd:TRICE line %b (%%b)\n", -2 );
    break; case __LINE__: TRICE( id( 2606), "rd:TRICE line %o (%%o)\n", -3 );
    break; case __LINE__: TRICE( id( 7421), "rd:TRICE line %x (%%x)\n", -4 );
    break; case __LINE__: TRICE( id( 3963), "rd:TRICE line %d (%%d)\n", -5 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 0 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 1 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 2 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 3 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 4 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 5 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 6 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 7 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 8 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 9 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 10 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 11 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 12 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 13 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 14 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 15 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 16 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 17 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 18 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 19 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 120 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 121 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 122 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 123 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 124 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 125 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 126 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 127 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 128 );
    break; case __LINE__: TRICE_N( id( 4770), "msg:%s\n", A, 129 );
    break; case __LINE__: exampleOfManualJSONencoding();
    break; case __LINE__: exampleOfBuffersAndFunctions();
    break; case __LINE__: exampleOfManualSerialization();

    break; case __LINE__: 
#ifdef XTEA_ENCRYPT_KEY
        TRICE0 (Id( 6463), "--------------------------------------------------\n" );
        {
            uint32_t by[8] = {1,2,3,4,5,6};
            int count = (sizeof(by) + 7) & ~7; // only multiple of 8 encryptable
            TRICE32(  Id( 7881), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
            TRICE32( id( 6649), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
            XTEAEncrypt(by, count>>2);
            TRICE32( id( 6901), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
            TRICE32(  Id( 7140), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
            #ifdef XTEA_DECRYPT
            TRICE16_1( id( 4887), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
            XTEADecrypt(by, count>>2);
            TRICE16_1( id( 6241), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
            #endif
            TRICE32(  Id( 2186), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
        }
        TRICE0 (Id( 7158), "--------------------------------------------------\n" );
#endif

    break; case __LINE__: showTriceDepth();
        break;
        default:
        break;
    }
}




//! SCOPY is a helper macro for struct serialization.
#define SCOPY( element ) do{ char* n = #element; int size = sizeof( src->element ); memcpy( p, &(src->element), size ); p += size; TRICE_S( id( 5135), "rd:sizeof(%8s)", n ); TRICE( id( 6144), " = %d\n", size);}while(0);

    //! DCOPY is a helper macro for struct deserialization.
#define DCOPY( element ) do{ char* n = #element; int size = sizeof( dst->element ); memcpy( &(dst->element), p, size ); p += size; TRICE_S( id( 5152), "rd:sizeof(%8s)", n ); TRICE( id( 3355), " = %d\n", size);}while(0);


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
    TRICE ( id( 5410), "inf: Tryout tx struct:" );
    TRICE_B( id( 6468), " %02x ", &tx, sizeof(tx) );
    TRICE( id( 2822), "\n" );
    
    TRICE ( id( 2706), "inf: Tryout buffer:" );
    TRICE_B( id( 3396), " %02x ", dst, len ); //lint !e670
    TRICE( id( 4774), "\n" );

    src = dst; // "data transfer"
    
    len = deserializeTryout( &rx, src );
    TRICE ( id( 7187), "inf: Tryout rx struct:" );
    TRICE_B( id( 6577), " %02x ", &rx, sizeof(rx) );
    TRICE( id( 5383), "\n" );

    TRICE( id( 5237), "inf:sizeOf(Tryout) = %d, buffer length = %d\n", sizeof(tx), len );
    TRICE8_F( id( 6773), "info:TryoutStructFunction", &tx, sizeof(tx) );
    TRICE8_F( id( 3543), "info:TryoutBufferFunction", dst, len ); //lint !e670
}

static void exampleOfManualJSONencoding(void){
    typedef  struct {
    int Apple, Birn;
    float Fish;
    } Ex_t;
    Ex_t Ex = { -1, 2, (float)2.781 };
    TRICE( id( 7582), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish) );
}

static void exampleOfBuffersAndFunctions(void){
    static int8_t   b8[24] = { 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11, 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11 };
    static int16_t b16[] = { 0, -1, -2, 0x3344 };
    static int32_t b32[] = { 0, -1, -2, 0x33445555};
    static int64_t b64[4] = { 0, -1, -2, 0x3344555566666666 };
    char* s = "abcde 12345";
    uint32_t len;

    TRICE_S( id( 1436), "msg:With TRICE_S:%s\n", s );
    len = strlen(s);
    TRICE_N( id( 3769), "sig:With TRICE_N:%s\n", s, len );
    TRICE32( id( 6888), "att:len=%u:With TRICE_B:\n", len);
    TRICE_B( id( 1587), "  %02x", s, len );
    TRICE( id( 3130), "\n" );
    TRICE_B( id( 6145), "%4d", s, len );
    TRICE( id( 7051), "\n" );

    TRICE( id( 7214), "notice:TRICE_B example: " );
    TRICE_B( id( 1238), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );
    TRICE( id( 6623), "\n" );
    TRICE( id( 4383), "notice:TRICE8_B example:" );
    TRICE8_B( id( 5469), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );
    TRICE( id( 2454), "\n" );
    TRICE( id( 4315), "notice:TRICE16_B example:" );
    TRICE16_B( id( 7019), "  %04x", b16, sizeof(b16)/sizeof(int16_t) );
    TRICE( id( 6717), "\n" );
    TRICE( id( 5766), "notice:TRICE32_B example:" );
    TRICE32_B( id( 7870), "  %08x", b32, sizeof(b32)/sizeof(int32_t) );
    TRICE( id( 5095), "\n" );
    TRICE( id( 5782), "notice:TRICE64_B example:" );
    TRICE64_B( id( 1951), "  %016x", b64, sizeof(b64)/sizeof(int64_t) );
    TRICE( id( 7721), "\n" );    

    TRICE( id( 6381), "notice:TRICE_F example: " );
    TRICE_F( id( 2844), "info:FunctionNameW",   b8,  sizeof(b8) /sizeof(int8_t) );
    //shown as: TRICE_F example: FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
    
    TRICE( id( 3434), "notice:TRICE8_F example:" );
    TRICE8_F(  Id( 6950), "info:FunctionNameW",   b8,  sizeof(b8) /sizeof(int8_t) );
    //shown as: TRICE8_F example:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
    
    TRICE( id( 4491), "notice:TRICE16_F example:" );
    TRICE16_F( id( 3620), "sig:FunctionNameX",    b16, sizeof(b16)/sizeof(int16_t) );
    //shown as: TRICE16_F example:FunctionNameX(0000)(ffff)(fffe)(3344) 

    TRICE( id( 6999), "notice:TRICE32_F example:" );
    TRICE32_F( id( 2711), "diag:FunctionNameY",   b32, sizeof(b32)/sizeof(int32_t) );
    //shown as: TRICE32_F example:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)

    TRICE( id( 7286), "notice:TRICE64_F example:" );
    TRICE64_F( id( 1484), "fatal:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );
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
