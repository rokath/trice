/*! \file triceCheck.c
\brief trices for tool evaluation
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include <float.h>
//#define TRICE_OFF // enable this line to disable trice code generation in this file object
#include "trice.h"

// Do **NOT** edit triceCheck.c!!!
//
// Edit file triceCheck.txt instead and call updateTestData.sh then.
// The strings behind "//exp:" are the expected result for each line (-color=none)


//  static int32_t FloatToInt32( float f );
//  static int64_t DoubleToInt64( double f );
//  static void exampleOfManualSerialization( void );
//  static void exampleOfManualJSONencoding(void);
//  static void exampleOfBuffersAndFunctions(void);

//  static void showTriceDepth( void ){
//      #if TRICE_MODE == TRICE_DOUBLE_BUFFER
//      {
//          uint16_t triceDepth = TriceDepth();
//          uint16_t triceDepthMax = TriceDepthMax();
//          TRice16( iD( 4497), "MSG: ?� STOP  TriceDepth = %d, TriceDepthMax = %d + %4u of %d\n", triceDepth, TRICE_DATA_OFFSET, triceDepthMax-TRICE_DATA_OFFSET, TRICE_HALF_BUFFER_SIZE );
//      } 
//      #endif
//      #if TRICE_MODE == TRICE_STREAM_BUFFER
//          TRice( iD( 7181), "MSG:triceFifoDepthMax = %d of max %d, triceStreamBufferDepthMax = %d of max %d\n", triceFifoDepthMax, TRICE_FIFO_ELEMENTS, triceStreamBufferDepthMax, TRICE_BUFFER_SIZE );
//      #endif
//  }

void TriceLogDepthMax( void ){
    #if TRICE_MODE == TRICE_DOUBLE_BUFFER
        size_t tdm = TriceDepthMax();
        if( tdm <= TRICE_HALF_BUFFER_SIZE ){
            TRice16( iD( 6854), "diag:TriceDepthMax =%4u of %d\n", tdm, TRICE_HALF_BUFFER_SIZE );
        }else{
            TRice16( iD( 3318), "err:TriceDepthMax =%4u of %d (overflow!)\n", tdm, TRICE_HALF_BUFFER_SIZE );
        }
    #endif
}

//! TriceCheck performs trice code sequence n.
//! TriceCheck writes out all types of trices with fixed values for testing
//! \details One trice has one subtrace, if param size max 2 bytes. 
//! Traces with more bytes as parameter consist of several subtraces.
//! Each trice line needs to have a commented // "string" with its expected output.
//! The ID values must be in file triceCheck.c, because it is compiled first and trice update runs later.
// This function is also called from Go for tests.
void TriceCheck(int n) {
    //  char* A = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    //  char* s = "AAAAAAAAAAAA";
    //  float  x = (float)1089.6082763671875; // 0x44883377
    //  double y = 518.0547492508867; // 0x4080307020601050

    switch( n ){
        default:
        break; case __LINE__: trice8   ( iD( 5634),         "value=%x", 0x55 );      //exp: time:        value=55
      //break; case __LINE__: trice8   ( id( 3209),  "value=%x", 0x55 );      //exp: time:        value=55

        break; case __LINE__: TRICE8   ( id( 4652), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE8_1 ( id( 2756), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE8   ( Id( 3647), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE8_1 ( Id( 4667), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE8   ( ID( 1792), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE8_1 ( ID( 1344), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice8   ( iD( 2718), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice8_1 ( iD( 2495), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice8   ( iD( 1283), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice8_1 ( iD( 2610), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice8   ( iD( 7393), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice8_1 ( iD( 1304), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        
        break; case __LINE__: TRICE16  ( id( 5902), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE16_1( id( 3508), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE16  ( Id( 7215), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE16_1( Id( 4839), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE16  ( ID( 2840), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE16_1( ID( 6417), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice16  ( iD( 5473), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice16_1( iD( 1673), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice16  ( iD( 1472), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice16_1( iD( 2516), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice16  ( iD( 2139), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice16_1( iD( 5425), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
		
        break; case __LINE__: TRICE32  ( id( 2078), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE32_1( id( 6366), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE32  ( Id( 7901), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE32_1( Id( 2731), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE32  ( ID( 6745), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE32_1( ID( 5986), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice32  ( iD( 7435), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice32_1( iD( 2978), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice32  ( iD( 2262), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice32_1( iD( 3155), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice32  ( iD( 3309), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice32_1( iD( 2339), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        
        break; case __LINE__: TRICE64  ( id( 7015), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE64_1( id( 1889), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE64  ( Id( 6246), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE64_1( Id( 6837), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE64  ( ID( 2181), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE64_1( ID( 6459), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice64  ( iD( 4350), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice64_1( iD( 5295), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice64  ( iD( 7798), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice64_1( iD( 6345), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice64  ( iD( 1985), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice64_1( iD( 7814), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200

        // mixed 16-bit and 32-bit timestamp //exp: time:        value=-2s
        break; case __LINE__: TRice8_1 ( 7009, "msg:value=%d\n", -1 );// trice tool modified code for compilation
        break; case __LINE__: Trice8_2 ( 5664, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
        break; case __LINE__: TRice8_3 ( 2103, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
        break; case __LINE__: Trice8_4 ( 7087, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
        break; case __LINE__: TRice8_5 ( 4931, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
        break; case __LINE__: Trice8_6 ( 2621, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
        break; case __LINE__: TRice8_7 ( 3488, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
        break; case __LINE__: Trice8_8 ( 4329, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
        break; case __LINE__: TRice8_9 ( 6469, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
        break; case __LINE__: Trice8_10( 6335, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
        break; case __LINE__: TRice8_11( 2410, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
        break; case __LINE__: Trice8_12( 2070, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

        // normal use cases (3 macros)
      //break; case __LINE__: trice      ( iD( 2704), "value=%d\n", -2  ); //exp: time:        value=-2
      //break; case __LINE__: Trice      ( iD( 3850), "value=%d\n", -2  ); //exp: time:    1616value=-2
      //break; case __LINE__: TRice      ( iD( 4525), "value=%d\n", -2  ); //exp: time:32323232value=-2
      //break; case __LINE__: trice_1    ( iD( 6707), "value=%d\n", -2  ); //exp: time:        value=-2
      //break; case __LINE__: Trice_1    ( iD( 1408), "value=%d\n", -2  ); //exp: time:    1616value=-2
      //break; case __LINE__: TRice_1    ( iD( 6289), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 8-bit data
        break; case __LINE__: trice8     ( iD( 6639), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice8     ( iD( 6132), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice8     ( iD( 3065), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice8_1   ( iD( 1396), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice8_1   ( iD( 5668), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice8_1   ( iD( 6986), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 16-bit data
        break; case __LINE__: trice16    ( iD( 1186), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice16    ( iD( 3600), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice16    ( iD( 3366), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice16_1  ( iD( 5102), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice16_1  ( iD( 3690), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice16_1  ( iD( 6245), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 32-bit data
        break; case __LINE__: trice32    ( iD( 5401), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice32    ( iD( 6661), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice32    ( iD( 7725), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice32_1  ( iD( 7256), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice32_1  ( iD( 7816), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice32_1  ( iD( 6161), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 64-bit data
        break; case __LINE__: trice64    ( iD( 3908), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice64    ( iD( 5320), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice64    ( iD( 6386), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice64_1  ( iD( 5008), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice64_1  ( iD( 3165), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice64_1  ( iD( 7805), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // legacy use cases
      //break; case __LINE__: TRICE      ( ID( 7745),        "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: TRICE      ( id( 3861), "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: TRICE      ( Id( 3451), "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: TRICE      ( ID( 2183), "value=%d\n", -2 ); //exp: time:32323232value=-2

        // default stamp
      //break; case __LINE__: TRICE8     ( ID( 5935),        "value=%d\n", -2 ); //exp: time:        value=-2
      //break; case __LINE__: TRICE8_1   ( ID( 1444),        "value=%d\n", -2 ); //exp: time:        value=-2

        // no stamp
        break; case __LINE__: trice8     ( iD( 1364),        "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: trice8_1   ( iD( 3782),        "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: TRICE8     ( id( 2880), "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: TRICE8_1   ( id( 7546), "value=%d\n", -2 ); //exp: time:        value=-2

        // 16-bit stamp:
        break; case __LINE__: Trice8     ( iD( 2094),        "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: Trice8_1   ( iD( 1645),        "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: TRICE8     ( Id( 4537), "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: TRICE8_1   ( Id( 4473), "value=%d\n", -2 ); //exp: time:    1616value=-2
          
        // 32-bit stamp:
        break; case __LINE__: TRice8     ( iD( 7482),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRice8_1   ( iD( 3348),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE8     ( ID( 3277), "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE8_1   ( ID( 2353), "value=%d\n", -2 ); //exp: time:32323232value=-2

        break;
    }
}
