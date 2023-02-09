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
        TRice16( iD( 3702), "MSG: ?� STOP  TriceDepth = %d, TriceDepthMax = %d + %4u of %d\n", triceDepth, TRICE_DATA_OFFSET, triceDepthMax-TRICE_DATA_OFFSET, TRICE_HALF_BUFFER_SIZE );
    } 
    #endif
    #if TRICE_MODE == TRICE_STREAM_BUFFER
        TRice( iD( 5679), "MSG:triceFifoDepthMax = %d of max %d, triceStreamBufferDepthMax = %d of max %d\n", triceFifoDepthMax, TRICE_FIFO_ELEMENTS, triceStreamBufferDepthMax, TRICE_BUFFER_SIZE );
    #endif
}

void TriceLogDepthMax( void ){
    #if TRICE_MODE == TRICE_DOUBLE_BUFFER
        size_t tdm = TriceDepthMax();
        if( tdm <= TRICE_HALF_BUFFER_SIZE ){
            TRice16( iD( 1275), "diag:TriceDepthMax =%4u of %d\n", tdm, TRICE_HALF_BUFFER_SIZE );
        }else{
            TRice16( iD( 2032), "err:TriceDepthMax =%4u of %d (overflow!)\n", tdm, TRICE_HALF_BUFFER_SIZE );
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
    char* A = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    char* s = "AAAAAAAAAAAA";
    float  x = (float)1089.6082763671875; // 0x44883377
    double y = 518.0547492508867; // 0x4080307020601050

    switch( n ){
        default:
        break; case __LINE__: trice8   ( iD( 4597),         "value=%x", 0x55 );      //exp: time:        value=55
        break; case __LINE__: trice8   ( iD( 2636),  "value=%x", 0x88 );      //exp: time:        value=88

        break; case __LINE__: TRICE8   ( id( 7948), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE8_1 ( id( 7653), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE8   ( Id( 2319), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE8_1 ( Id( 3148), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE8   ( ID( 4069), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE8_1 ( ID( 2846), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice8   ( iD( 6901), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice8_1 ( iD( 3752), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice8   ( iD( 1550), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice8_1 ( iD( 4416), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice8   ( iD( 6738), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice8_1 ( iD( 7472), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        
        break; case __LINE__: TRICE16  ( id( 6454), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE16_1( id( 1545), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE16  ( Id( 7805), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE16_1( Id( 5724), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE16  ( ID( 7893), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE16_1( ID( 6005), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice16  ( iD( 5677), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice16_1( iD( 7986), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice16  ( iD( 2052), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice16_1( iD( 7237), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice16  ( iD( 4696), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice16_1( iD( 7795), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
		
        break; case __LINE__: TRICE32  ( id( 6033), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE32_1( id( 1126), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE32  ( Id( 1200), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE32_1( Id( 6312), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE32  ( ID( 3896), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE32_1( ID( 6080), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice32  ( iD( 6607), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice32_1( iD( 2146), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice32  ( iD( 3401), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice32_1( iD( 4870), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice32  ( iD( 1778), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice32_1( iD( 1128), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        
        break; case __LINE__: TRICE64  ( id( 7834), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE64_1( id( 7318), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE64  ( Id( 1046), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE64_1( Id( 7453), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE64  ( ID( 3376), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE64_1( ID( 5585), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice64  ( iD( 1534), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice64_1( iD( 1758), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice64  ( iD( 3857), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice64_1( iD( 3844), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice64  ( iD( 6031), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice64_1( iD( 5403), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200

        // mixed 16-bit and 32-bit timestamp 
        break; case __LINE__: TRice8( iD( 1709), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice8( iD( 4987), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice8( iD( 2287), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice8( iD( 3383), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8( iD( 3468), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8( iD( 3267), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8( iD( 6109), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8( iD( 2660), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8( iD( 6795), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8( iD( 5471), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8( iD( 1722), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8( iD( 2700), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16( iD( 7906), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice16( iD( 2010), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice16( iD( 1835), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice16( iD( 3110), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16( iD( 4814), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16( iD( 7720), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16( iD( 6277), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16( iD( 4997), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16( iD( 7063), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16( iD( 7090), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16( iD( 4787), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16( iD( 2709), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32( iD( 4574), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice32( iD( 4495), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice32( iD( 5434), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice32( iD( 4292), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32( iD( 5528), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32( iD( 3717), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32( iD( 2961), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32( iD( 1390), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32( iD( 5817), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32( iD( 4736), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32( iD( 2276), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32( iD( 3439), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64( iD( 1092), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice64( iD( 4216), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice64( iD( 4206), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice64( iD( 4371), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64( iD( 4491), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64( iD( 7202), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64( iD( 1746), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64( iD( 7515), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64( iD( 4843), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64( iD( 7268), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64( iD( 4740), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64( iD( 1743), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice8_1 ( iD( 4531), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice8_2 ( iD( 6858), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice8_3 ( iD( 7588), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice8_4 ( iD( 7469), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8_5 ( iD( 1663), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8_6 ( iD( 5331), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8_7 ( iD( 5014), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8_8 ( iD( 6442), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8_9 ( iD( 1736), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8_10( iD( 3357), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8_11( iD( 5925), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8_12( iD( 3052), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16_1 ( iD( 6772), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice16_2 ( iD( 7755), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice16_3 ( iD( 5117), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice16_4 ( iD( 5457), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16_5 ( iD( 5165), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16_6 ( iD( 3260), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16_7 ( iD( 6149), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16_8 ( iD( 3213), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16_9 ( iD( 4481), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16_10( iD( 5419), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16_11( iD( 3951), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16_12( iD( 3854), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32_1 ( iD( 4879), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice32_2 ( iD( 5066), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice32_3 ( iD( 4920), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice32_4 ( iD( 1449), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32_5 ( iD( 1472), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32_6 ( iD( 2598), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32_7 ( iD( 2803), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32_8 ( iD( 3341), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32_9 ( iD( 1644), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32_10( iD( 7901), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32_11( iD( 6262), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32_12( iD( 2826), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64_1 ( iD( 5211), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice64_2 ( iD( 6988), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice64_3 ( iD( 5884), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice64_4 ( iD( 3647), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64_5 ( iD( 7527), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64_6 ( iD( 5453), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64_7 ( iD( 1659), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64_8 ( iD( 4124), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64_9 ( iD( 6376), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64_10( iD( 3767), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64_11( iD( 4899), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64_12( iD( 5568), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        // normal use cases (3 macros)
      //break; case __LINE__: trice      ( iD( 4028), "value=%d\n", -2  ); //exp: time:        value=-2
      //break; case __LINE__: Trice      ( iD( 2023), "value=%d\n", -2  ); //exp: time:    1616value=-2
      //break; case __LINE__: TRice      ( iD( 3324), "value=%d\n", -2  ); //exp: time:32323232value=-2
      //break; case __LINE__: trice_1    ( iD( 7828), "value=%d\n", -2  ); //exp: time:        value=-2
      //break; case __LINE__: Trice_1    ( iD( 6268), "value=%d\n", -2  ); //exp: time:    1616value=-2
      //break; case __LINE__: TRice_1    ( iD( 5396), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 8-bit data
        break; case __LINE__: trice8     ( iD( 6637), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice8     ( iD( 1858), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice8     ( iD( 4130), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice8_1   ( iD( 2698), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice8_1   ( iD( 2190), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice8_1   ( iD( 4289), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 16-bit data
        break; case __LINE__: trice16    ( iD( 1732), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice16    ( iD( 1002), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice16    ( iD( 4351), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice16_1  ( iD( 6755), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice16_1  ( iD( 7149), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice16_1  ( iD( 3565), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 32-bit data
        break; case __LINE__: trice32    ( iD( 4756), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice32    ( iD( 5769), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice32    ( iD( 6598), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice32_1  ( iD( 1717), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice32_1  ( iD( 3530), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice32_1  ( iD( 3810), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 64-bit data
        break; case __LINE__: trice64    ( iD( 1916), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice64    ( iD( 7551), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice64    ( iD( 7059), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice64_1  ( iD( 5478), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice64_1  ( iD( 4578), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice64_1  ( iD( 1388), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // legacy use cases
        break; case __LINE__: TRICE      ( ID( 2211),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE      ( id( 6587), "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: TRICE      ( Id( 3841), "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: TRICE      ( ID( 2039), "value=%d\n", -2 ); //exp: time:32323232value=-2

        // default stamp
        break; case __LINE__: TRICE8     ( ID( 2007),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE8_1   ( ID( 6001),        "value=%d\n", -2 ); //exp: time:32323232value=-2

        // no stamp
        break; case __LINE__: trice8     ( iD( 4193),        "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: trice8_1   ( iD( 3950),        "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: TRICE8     ( id( 2694), "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: TRICE8_1   ( id( 6546), "value=%d\n", -2 ); //exp: time:        value=-2

        // 16-bit stamp:
        break; case __LINE__: Trice8     ( iD( 7122),        "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: Trice8_1   ( iD( 6178),        "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: TRICE8     ( Id( 4968), "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: TRICE8_1   ( Id( 7528), "value=%d\n", -2 ); //exp: time:    1616value=-2
          
        // 32-bit stamp:
        break; case __LINE__: TRice8     ( iD( 3131),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRice8_1   ( iD( 3734),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE8     ( ID( 7833), "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE8_1   ( ID( 6007), "value=%d\n", -2 ); //exp: time:32323232value=-2




        break; case __LINE__: TRICE( Id( 1351), "MSG:1/11 = %g\n", aFloat( 1.0/11 ) ); //exp: time:    1616MSG:1/11 = 0.09090909
        break; case __LINE__: { //exp: time:32323232msg:x = 5.934 = 5.934, 5.934
                              float a = (float)5.934;
                              float b = a + ((a > 0) ? 0.0005f : -0.0005f);
                              int c = (int)b;
                              int d = (int)(b * 1000) % 1000;
                              int e = (int)(1000 * (float)(a - c)); 
                              TRICE( ID( 4592), "msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e ); 
                              }
        break; case __LINE__: TRICE( id( 5337), "FATAL:magenta+b:red\n" );           // exp: time:        FATAL:magenta+b:red
        break; case __LINE__: TRICE( id( 3120), "CRITICAL:red+i:default+h\n" );      // exp: time:        CRITICAL:red+i:default+h
        break; case __LINE__: TRICE( id( 5966), "EMERGENCY:red+i:blue\n" );          // exp: time:        EMERGENCY:red+i:blue
        break; case __LINE__: TRICE( id( 6432), "ERROR:11:red\n" );                  // exp: time:        ERROR:11:red
        break; case __LINE__: TRICE( id( 4737), "WARNING:11+i:red\n" );              // exp: time:        WARNING:11+i:red
        break; case __LINE__: TRICE( id( 4903), "ATTENTION:11:green\n" );            // exp: time:        ATTENTION:11:green
        break; case __LINE__: TRICE( id( 2927), "INFO:cyan+b:default+h\n" );         // exp: time:        INFO:cyan+b:default+h
        break; case __LINE__: TRICE( id( 4300), "DEBUG:130+i\n" );                   // exp: time:        DEBUG:130+i
        break; case __LINE__: TRICE( id( 4838), "TRACE:default+i:default+h\n" );     // exp: time:        TRACE:default+i:default+h
        break; case __LINE__: TRICE( id( 6119), "TIME:blue+i:blue+h\n" );            // exp: time:        TIME:blue+i:blue+h
        break; case __LINE__: TRICE( id( 5116), "MESSAGE:green+h:black\n" );         // exp: time:        MESSAGE:green+h:black
        break; case __LINE__: TRICE( id( 5275), "READ:black+i:yellow+h\n" );         // exp: time:        READ:black+i:yellow+h
        break; case __LINE__: TRICE( id( 1826), "WRITE:black+u:yellow+h\n" );        // exp: time:        WRITE:black+u:yellow+h
        break; case __LINE__: TRICE( id( 6776), "RECEIVE:black+h:black\n" );         // exp: time:        RECEIVE:black+h:black
        break; case __LINE__: TRICE( id( 3476), "TRANSMIT:black:black+h\n" );        // exp: time:        TRANSMIT:black:black+h
        break; case __LINE__: TRICE( id( 2001), "DIAG:yellow+i:default+h\n" );       // exp: time:        DIAG:yellow+i:default+h
        break; case __LINE__: TRICE( id( 3394), "INTERRUPT:magenta+i:default+h\n" ); // exp: time:        INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE( id( 3531), "SIGNAL:118+i\n" );                  // exp: time:        SIGNAL:118+i
        break; case __LINE__: TRICE( id( 1422), "TEST:yellow+h:black\n" );           // exp: time:        TEST:yellow+h:black
        break; case __LINE__: TRICE( id( 5158), "DEFAULT:off\n" );                   // exp: time:        DEFAULT:off
        break; case __LINE__: TRICE( id( 2217), "NOTICE:blue:white+h\n" );           // exp: time:        NOTICE:blue:white+h
        break; case __LINE__: TRICE( id( 4466), "ALERT:magenta:magenta+h\n" );       // exp: time:        ALERT:magenta:magenta+h
        break; case __LINE__: TRICE( id( 2874), "ASSERT:yellow+i:blue\n" );          // exp: time:        ASSERT:yellow+i:blue
        break; case __LINE__: TRICE( id( 6079), "ALARM:red+i:white+h\n" );           // exp: time:        ALARM:red+i:white+h
        break; case __LINE__: TRICE( id( 7362), "CYCLE:blue+i:default+h\n" );        // exp: time:        CYCLE:blue+i:default+h
        break; case __LINE__: TRICE( id( 3314), "VERBOSE:blue:default\n" );          // exp: time:        VERBOSE:blue:default
        break; case __LINE__: TRICE( id( 6272), "fatal:magenta+b:red\n" );           // exp: time:        fatal:magenta+b:red
        break; case __LINE__: TRICE( id( 7573), "critical:red+i:default+h\n" );      // exp: time:        critical:red+i:default+h
        break; case __LINE__: TRICE( id( 1134), "emergency:red+i:blue\n" );          // exp: time:        emergency:red+i:blue
        break; case __LINE__: TRICE( id( 3916), "error:11:red\n" );                  // exp: time:        error:11:red
        break; case __LINE__: TRICE( id( 1913), "warning:11+i:red\n" );              // exp: time:        warning:11+i:red
        break; case __LINE__: TRICE( id( 4407), "attention:11:green\n" );            // exp: time:        attention:11:green
        break; case __LINE__: TRICE( id( 6283), "info:cyan+b:default+h\n" );         // exp: time:        info:cyan+b:default+h
        break; case __LINE__: TRICE( id( 2133), "debug:130+i\n" );                   // exp: time:        debug:130+i
        break; case __LINE__: TRICE( id( 6935), "trace:default+i:default+h\n" );     // exp: time:        trace:default+i:default+h
        break; case __LINE__: TRICE( id( 3189), "time:blue+i:blue+h\n" );            // exp: time:        time:blue+i:blue+h
        break; case __LINE__: TRICE( id( 6873), "message:green+h:black\n" );         // exp: time:        message:green+h:black
        break; case __LINE__: TRICE( id( 5007), "read:black+i:yellow+h\n" );         // exp: time:        read:black+i:yellow+h
        break; case __LINE__: TRICE( id( 7208), "write:black+u:yellow+h\n" );        // exp: time:        write:black+u:yellow+h
        break; case __LINE__: TRICE( id( 2790), "receive:black+h:black\n" );         // exp: time:        receive:black+h:black
        break; case __LINE__: TRICE( id( 6329), "transmit:black:black+h\n" );        // exp: time:        transmit:black:black+h
        break; case __LINE__: TRICE( id( 6762), "diag:yellow+i:default+h\n" );       // exp: time:        diag:yellow+i:default+h
        break; case __LINE__: TRICE( id( 6575), "interrupt:magenta+i:default+h\n" ); // exp: time:        interrupt:magenta+i:default+h
        break; case __LINE__: TRICE( id( 2612), "signal:118+i\n" );                  // exp: time:        signal:118+i
        break; case __LINE__: TRICE( id( 6379), "test:yellow+h:black\n" );           // exp: time:        test:yellow+h:black
        break; case __LINE__: TRICE( id( 3800), "default:off\n" );                   // exp: time:        default:off
        break; case __LINE__: TRICE( id( 6163), "notice:blue:white+h\n" );           // exp: time:        notice:blue:white+h
        break; case __LINE__: TRICE( id( 6140), "allert:magenta:magenta+h\n" );      // exp: time:        allert:magenta:magenta+h
        break; case __LINE__: TRICE( id( 2608), "assert:yellow+i:blue\n" );          // exp: time:        assert:yellow+i:blue
        break; case __LINE__: TRICE( id( 3732), "alarm:red+i:white+h\n" );           // exp: time:        alarm:red+i:white+h
        break; case __LINE__: TRICE( id( 2183), "cycle:blue+i:default+h\n" );        // exp: time:        cycle:blue+i:default+h
        break; case __LINE__: TRICE( id( 4463), "verbose:blue:default\n" );          // exp: time:        verbose:blue:default
        break; case __LINE__: TRICE( id( 5768), "dbg:Hi!\n" );                       // exp: time:        dbg:Hi!    
        break; case __LINE__: TRICE( id( 6635), "dbg:\tHi!\n" );                     
        break; case __LINE__: TRICE( id( 3434), "dbg:\t\tHi!\n" );                   
        break; case __LINE__: TRICE( id( 4259), "dbg:\t\t\tHi!\n" );                 
        break; case __LINE__: TRICE( id( 2504), "dbg:\\tHi!\n" );                    
        break; case __LINE__: TRICE( Id( 4401), "dbg:\aHi!\n" ); // sound!
        break; case __LINE__: TRICE( Id( 5023), "dbg:\a\aHi!\n" ); // sound!
        break; case __LINE__: TRICE( Id( 1323), "dbg:\a\a\aHi!\n" ); // sound!
        break; case __LINE__: TRICE( Id( 1356), "dbg:\\aHi!\n" );
        break; case __LINE__: TRICE( Id( 2261), "err:ERROR!\a\n" ); // sound!

        break; case __LINE__: TRICE_S( id( 5518), "sig:TRICE_S=%s\n", s );            // exp: time:        sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N( id( 1848), "sig:TRICE_N=%s\n", s, strlen(s) ); // exp: time:        sig:TRICE_N=AAAAAAAAAAAA

        break; case __LINE__: TRICE  ( id( 1574), "rd:%E (%%E)\n",  aFloat(-555555555.5555555555) ); // exp: time:        rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE  ( id( 7838), "rd:%F (%%F)\n",  aFloat(-555555555.5555555555) ); // exp: time:        rd:-555555584.000000 (%F)
        break; case __LINE__: TRICE  ( id( 5036), "rd:%G (%%G)\n",  aFloat(-555555555.5555555555) ); // exp: time:        rd:-5.555556E+08 (%G)
        break; case __LINE__: TRICE64( id( 7301), "rd:%E (%%E)\n", aDouble(-555555555.5555555555) ); // exp: time:        rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE64( id( 5812), "rd:%F (%%F)\n", aDouble(-555555555.5555555555) ); // exp: time:        rd:-555555555.555556 (%F)
        break; case __LINE__: TRICE64( id( 7225), "rd:%G (%%G)\n", aDouble(-555555555.5555555555) ); // exp: time:        rd:-5.555555555555556E+08 (%G)

        break; case __LINE__: TRICE  ( id( 4453), "rd:%e (%%e)\n",  aFloat(-555555555.5555555555) ); // exp: time:        rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE  ( id( 4492), "rd:%f (%%f)\n",  aFloat(-555555555.5555555555) ); // exp: time:        rd:-555555584.000000 (%f)
        break; case __LINE__: TRICE  ( id( 6143), "rd:%g (%%g)\n",  aFloat(-555555555.5555555555) ); // exp: time:        rd:-5.555556e+08 (%g)
        break; case __LINE__: TRICE64( id( 4298), "rd:%e (%%e)\n", aDouble(-555555555.5555555555) ); // exp: time:        rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE64( id( 2518), "rd:%f (%%f)\n", aDouble(-555555555.5555555555) ); // exp: time:        rd:-555555555.555556 (%f)
        break; case __LINE__: TRICE64( id( 6113), "rd:%g (%%g)\n", aDouble(-555555555.5555555555) ); // exp: time:        rd:-5.555555555555556e+08 (%g)

        break; case __LINE__: TRICE32( id( 5696), "msg:%u (%%u)\n", -1 ); //exp: time:        msg:4294967295 (%u)
        break; case __LINE__: TRICE32( id( 1728), "msg:%b (%%b)\n", -1 ); //exp: time:        msg:11111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE32( id( 3662), "msg:%o (%%o)\n", -1 ); //exp: time:        msg:37777777777 (%o)
        break; case __LINE__: TRICE32( id( 1296), "msg:%O (%%O)\n", -1 ); //exp: time:        msg:0o37777777777 (%O)
        break; case __LINE__: TRICE32( id( 3372), "msg:%X (%%X)\n", -1 ); //exp: time:        msg:FFFFFFFF (%X)
        break; case __LINE__: TRICE32( id( 4243), "msg:%x (%%x)\n", -1 ); //exp: time:        msg:ffffffff (%x)
        break; case __LINE__: TRICE32( id( 2684), "msg:%d (%%d)\n", -1 ); //exp: time:        msg:-1 (%d)
        break; case __LINE__: TRICE64( id( 5618), "msg:%u (%%u)\n", -1 ); //exp: time:        msg:18446744073709551615 (%u)
        break; case __LINE__: TRICE64( id( 7951), "msg:%b (%%b)\n", -1 ); //exp: time:        msg:1111111111111111111111111111111111111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE64( id( 3182), "msg:%o (%%o)\n", -1 ); //exp: time:        msg:1777777777777777777777 (%o)
        break; case __LINE__: TRICE64( id( 7914), "msg:%O (%%O)\n", -1 ); //exp: time:        msg:0o1777777777777777777777 (%O)
        break; case __LINE__: TRICE64( id( 6704), "msg:%X (%%X)\n", -1 ); //exp: time:        msg:FFFFFFFFFFFFFFFF (%X)
        break; case __LINE__: TRICE64( id( 2627), "msg:%x (%%x)\n", -1 ); //exp: time:        msg:ffffffffffffffff (%x)
        break; case __LINE__: TRICE64( id( 6216), "msg:%d (%%d)\n", -1 ); //exp: time:        msg:-1 (%d)





        break; case __LINE__: TRICE( Id( 6158), "sig:Some time measurements\n" );
        break; case __LINE__: TRICE( Id( 7684), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( Id( 2717), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( Id( 4301), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( Id( 2687), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE64( Id( 2144), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( Id( 1875), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( Id( 4508), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( Id( 3167), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( Id( 3514), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( Id( 6603), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE64( Id( 2689), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( Id( 6083), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( Id( 5354), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( Id( 6295), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( Id( 3830), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( Id( 3220), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( Id( 7425), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( Id( 2545), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( Id( 6870),  "rd:TRICE32_1 line %d\n", __LINE__ );
        break; case __LINE__: TRICE64_1( Id( 1291),  "rd:TRICE64_1 %d\n", __LINE__ );
        break; case __LINE__: TRICE32_2( Id( 3922),  "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
        break; case __LINE__: TRICE64_2( Id( 3537),  "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );

    break; case __LINE__: TRICE( Id( 3201), "sig:TRICE with 1 to 12 values\n" );
    break; case __LINE__: TRICE( Id( 3636),"rd:TRICE %d\n", -1 );
    break; case __LINE__: TRICE( Id( 4922),"rd:TRICE %d, %d\n", -1, -2 );
    break; case __LINE__: TRICE( Id( 6393),"rd:TRICE %d, %d, %d\n", -1, -2, -3 );
    break; case __LINE__: TRICE( Id( 5554),"rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );
    break; case __LINE__: TRICE( Id( 6074),"rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
    break; case __LINE__: TRICE( Id( 2764),"rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
    break; case __LINE__: TRICE( Id( 4184),"rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
    break; case __LINE__: TRICE( Id( 5011),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
    break; case __LINE__: TRICE( Id( 7898),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
    break; case __LINE__: TRICE( Id( 3828),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
    break; case __LINE__: TRICE( Id( 5064),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
    break; case __LINE__: TRICE( Id( 4447),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE( Id( 4847), "sig:TRICE_n with 1 to 12 values\n" );
    break; case __LINE__: TRICE_1( Id( 7096),   "rd:TRICE_1 %d\n", 1 );
    break; case __LINE__: TRICE_2( Id( 4796),   "rd:TRICE_2 %d, %d\n", 1, 2 );
    break; case __LINE__: TRICE_3( Id( 6039),   "rd:TRICE_3 %d, %d, %d\n", 1, 2, 3 );
    break; case __LINE__: TRICE_4( Id( 4059),   "rd:TRICE_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
    break; case __LINE__: TRICE_5( Id( 3311),   "rd:TRICE_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
    break; case __LINE__: TRICE_6( Id( 7989),   "rd:TRICE_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
    break; case __LINE__: TRICE_7( Id( 6088),   "rd:TRICE_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
    break; case __LINE__: TRICE_8( Id( 1157),   "rd:TRICE_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
    break; case __LINE__: TRICE_9( Id( 7332),   "rd:TRICE_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
    break; case __LINE__: TRICE_10( Id( 1617),  "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    break; case __LINE__: TRICE_11( Id( 4071),  "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
    break; case __LINE__: TRICE_12( Id( 3014),  "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );				
  //break; case __LINE__: TRICE( Id( 7678), "sig:TRICE8 with 1 to 12 values\n" );
  //break; case __LINE__: TRICE8( Id( 1288),"rd:TRICE8 %d\n", -1 );
  //break; case __LINE__: TRICE8( Id( 6492),"rd:TRICE8 %d, %d\n", -1, -2 );
  //break; case __LINE__: TRICE8( Id( 3033),"rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE8( Id( 7569),"rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE8( Id( 7823),"rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE8( Id( 7171),"rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE8( Id( 1608),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE8( Id( 4759),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE8( Id( 1905),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE8( Id( 5970),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
  //break; case __LINE__: TRICE8( Id( 4649),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE8( Id( 5511),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE( Id( 5936), "sig:TRICE8_n with 1 to 12 values\n" );
  //break; case __LINE__: TRICE8_1( Id( 1019),   "rd:TRICE8_1 %d\n", 1 );
  //break; case __LINE__: TRICE8_2( Id( 3114),   "rd:TRICE8_2 %d, %d\n", 1, 2 );
  //break; case __LINE__: TRICE8_3( Id( 1404),   "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
  //break; case __LINE__: TRICE8_4( Id( 7136),   "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
  //break; case __LINE__: TRICE8_5( Id( 6401),   "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
  //break; case __LINE__: TRICE8_6( Id( 2503),   "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
  //break; case __LINE__: TRICE8_7( Id( 6092),   "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
  //break; case __LINE__: TRICE8_8( Id( 3342),   "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
  //break; case __LINE__: TRICE8_9( Id( 7437),   "rd:TRICE8_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
  //break; case __LINE__: TRICE8_10( Id( 1696),  "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  //break; case __LINE__: TRICE8_11( Id( 3658),  "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
  //break; case __LINE__: TRICE8_12( Id( 7680),  "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
  //break; case __LINE__: TRICE( Id( 3386), "sig:TRICE16 with 1 to 12 values\n" );
  //break; case __LINE__: TRICE16( Id( 5291),"rd:TRICE16 %d\n", -1 );
  //break; case __LINE__: TRICE16( Id( 1229),"rd:TRICE16 %d, %d\n", -1, -2 );
  //break; case __LINE__: TRICE16( Id( 6396),"rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE16( Id( 1429),"rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE16( Id( 2792),"rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE16( Id( 4439),"rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE16( Id( 6189),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE16( Id( 6736),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE16( Id( 3741),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE16( Id( 3349),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
  //break; case __LINE__: TRICE16( Id( 5427),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE16( Id( 4251),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE( Id( 3867), "sig:TRICE16_n with 1 to 12 values\n" );
  //break; case __LINE__: TRICE16_1( Id( 2911),  "rd:TRICE16_1 %d\n", 1 );
  //break; case __LINE__: TRICE16_2( Id( 4760),  "rd:TRICE16_2 %d, %d\n", 1, 2 );
  //break; case __LINE__: TRICE16_3( Id( 6599),  "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
  //break; case __LINE__: TRICE16_4( Id( 6995),  "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
  //break; case __LINE__: TRICE16_5( Id( 2390),  "rd:TRICE16_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
  //break; case __LINE__: TRICE16_6( Id( 3861),  "rd:TRICE16_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
  //break; case __LINE__: TRICE16_7( Id( 2892),  "rd:TRICE16_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
  //break; case __LINE__: TRICE16_8( Id( 5432),  "rd:TRICE16_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
  //break; case __LINE__: TRICE16_9( Id( 3058),  "rd:TRICE16_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
  //break; case __LINE__: TRICE16_10( Id( 1304), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  //break; case __LINE__: TRICE16_11( Id( 6522), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
  //break; case __LINE__: TRICE16_12( Id( 1186), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
  //break; case __LINE__: TRICE( Id( 6586), "sig:TRICE16 with 1 to 12 pointer\n" );
  //break; case __LINE__: TRICE16( Id( 7826),"rd:TRICE16 %p\n", -1 );
  //break; case __LINE__: TRICE16( Id( 4635),"rd:TRICE16 %p, %p\n", -1, -2 );
  //break; case __LINE__: TRICE16( Id( 4821),"rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
  //break; case __LINE__: TRICE16( Id( 6087),"rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE16( Id( 1241),"rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE16( Id( 6049),"rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE16( Id( 6819),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE16( Id( 5479),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE16( Id( 4164),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE16( Id( 1866),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
  //break; case __LINE__: TRICE16( Id( 3007),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE16( Id( 1934),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE( Id( 6081), "sig:TRICE16 with 1 to 12 hex\n" );
  //break; case __LINE__: TRICE16( Id( 3574),"rd:TRICE16 %X\n", -1 );
  //break; case __LINE__: TRICE16( Id( 4205),"rd:TRICE16 %X, %X\n", -1, -2 );
  //break; case __LINE__: TRICE16( Id( 3667),"rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
  //break; case __LINE__: TRICE16( Id( 5549),"rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE16( Id( 6871),"rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE16( Id( 2162),"rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE16( Id( 4650),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE16( Id( 2348),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE16( Id( 7942),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE16( Id( 6746),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
  //break; case __LINE__: TRICE16( Id( 5094),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE16( Id( 5819),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE( Id( 4227), "sig:TRICE32 with 1 to 12 values\n" );
    break; case __LINE__: TRICE32( Id( 4381),"rd:TRICE32 %d\n", -1 );
    break; case __LINE__: TRICE32( Id( 4042),"rd:TRICE32 %d, %d\n", -1, -2 );
    break; case __LINE__: TRICE32( Id( 3710),"rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );
    break; case __LINE__: TRICE32( Id( 6619),"rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );
    break; case __LINE__: TRICE32( Id( 3670),"rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
    break; case __LINE__: TRICE32( Id( 7117),"rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
    break; case __LINE__: TRICE32( Id( 4472),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
    break; case __LINE__: TRICE32( Id( 3483),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
    break; case __LINE__: TRICE32( Id( 3733),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
    break; case __LINE__: TRICE32( Id( 2606),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
    break; case __LINE__: TRICE32( Id( 4558),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
    break; case __LINE__: TRICE32( Id( 6318),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE( Id( 1749), "signal:TRICE32_n with 1 to 12 values\n" );
    break; case __LINE__: TRICE32_1( Id( 3061),  "rd:TRICE32_1 %d\n", 1 );
    break; case __LINE__: TRICE32_2( Id( 5438),  "rd:TRICE32_2 %d, %d\n", 1, 2 );
    break; case __LINE__: TRICE32_3( Id( 6527),  "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
    break; case __LINE__: TRICE32_4( Id( 4984),  "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
    break; case __LINE__: TRICE32_5( Id( 3083),  "rd:TRICE32_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
    break; case __LINE__: TRICE32_6( Id( 4885),  "rd:TRICE32_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
    break; case __LINE__: TRICE32_7( Id( 7258),  "rd:TRICE32_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
    break; case __LINE__: TRICE32_8( Id( 2868),  "rd:TRICE32_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
    break; case __LINE__: TRICE32_9( Id( 7289),  "rd:TRICE32_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
    break; case __LINE__: TRICE32_10( Id( 5263), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    break; case __LINE__: TRICE32_11( Id( 6497), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
    break; case __LINE__: TRICE32_12( Id( 4377), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
    break; case __LINE__: TRICE( Id( 3791), "sig:TRICE64 with 1 to 12 values\n" );
  //break; case __LINE__: TRICE64( Id( 3642),"rd:TRICE64 %d\n", -1 );
  //break; case __LINE__: TRICE64( Id( 4097),"rd:TRICE64 %d, %d\n", -1, -2 );
  //break; case __LINE__: TRICE64( Id( 7373),"rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE64( Id( 7456),"rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE64( Id( 4154),"rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE64( Id( 3326),"rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE64( Id( 7964),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE64( Id( 4917),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE64( Id( 3981),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE64( Id( 3612),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
  //break; case __LINE__: TRICE64( Id( 1952),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE64( Id( 1312),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE( Id( 6918), "sig:TRICE64_n with 1 to 12 values\n" );
  //break; case __LINE__: TRICE64_1( Id( 3360),  "rd:TRICE64_1 %d\n", 1 );
  //break; case __LINE__: TRICE64_2( Id( 4709),  "rd:TRICE64_2 %d, %d\n", 1, 2 );
  //break; case __LINE__: TRICE64_2( Id( 6028),  "rd:TRICE64_2 %x, %x\n", 0x1122334455667788, 0x0102030405060708 );
  //break; case __LINE__: TRICE64_3( Id( 5002),  "rd:TRICE64_3 %d, %d, %d\n", 1, 2, 3 );
  //break; case __LINE__: TRICE64_4( Id( 6525),  "rd:TRICE64_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
  //break; case __LINE__: TRICE64_5( Id( 1170),  "rd:TRICE64_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
  //break; case __LINE__: TRICE64_6( Id( 6721),  "rd:TRICE64_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
  //break; case __LINE__: TRICE64_7( Id( 2594),  "rd:TRICE64_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
  //break; case __LINE__: TRICE64_8( Id( 1290),  "rd:TRICE64_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
  //break; case __LINE__: TRICE64_9( Id( 1011),  "rd:TRICE64_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
  //break; case __LINE__: TRICE64_10( Id( 3819), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  //break; case __LINE__: TRICE64_11( Id( 6445), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
  //break; case __LINE__: TRICE64_12( Id( 3610), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
  //break; case __LINE__: TRICE( Id( 5503), "sig:Colors and ticks\n" );
  //break; case __LINE__: TRICE( Id( 7921), "--------------------------------------------------\n");
  //break; case __LINE__: TRICE( Id( 1924), "--------------------------------------------------\n");
  //break; case __LINE__: TRICE( Id( 1099), "dbg:12345 as 16bit is %016b\n", 12345);
  //break; case __LINE__: TRICE( Id( 1245), "--------------------------------------------------\n");
  //break; case __LINE__: TRICE( Id( 2798), "sig:This ASSERT error is just a demo and no real error:\n");
  //break; case __LINE__: TRICE( Id( 4537), "--------------------------------------------------\n");
  //break; case __LINE__: TRICE( Id( 7879), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id( 2976), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id( 6243), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id( 5699), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id( 7365), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id( 6734), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id( 1894), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id( 6954), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id( 3244), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id( 2537), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id( 5130), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id( 1537), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id( 5878), "time:TRICE32_1  message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id( 3895), "sig:Various ranges and formats\n" );
  //break; case __LINE__: TRICE8( Id( 1392), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( Id( 6055), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( Id( 1070), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( Id( 1252), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( Id( 5856), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( Id( 6570), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( Id( 3412), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( Id( 3994), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( Id( 1097), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE16(Id( 1768), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
  //break; case __LINE__: TRICE16(Id( 6629), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
  //break; case __LINE__: TRICE16(Id( 5510), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);
  //break; case __LINE__: TRICE32(Id( 1527), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
  //break; case __LINE__: TRICE32(Id( 1571), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
  //break; case __LINE__: TRICE32(Id( 2380), "att:TRICE32 %40b\n", 0xAAAAAAAA);
  //break; case __LINE__: TRICE64(Id( 7693), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
  //break; case __LINE__: TRICE64(Id( 5098), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
  //break; case __LINE__: TRICE64(Id( 5110), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
  //break; case __LINE__: TRICE( Id( 6284), "sig:Legacy TRICE8\n" );
  //break; case __LINE__: TRICE8_1(Id( 6321), "tst:TRICE8_1 %02x\n", 0xA1);
  //break; case __LINE__: TRICE8_2(Id( 5051), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
  //break; case __LINE__: TRICE8_3(Id( 2326), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
  //break; case __LINE__: TRICE8_4(Id( 2375), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
  //break; case __LINE__: TRICE8_5(Id( 7283), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
  //break; case __LINE__: TRICE8_6(Id( 1504), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
  //break; case __LINE__: TRICE8_7(Id( 6828), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
  //break; case __LINE__: TRICE8_8(Id( 6829), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
  //break; case __LINE__: TRICE8_1(Id( 7690), "tst:TRICE8_1 %d\n",                      -1);
  //break; case __LINE__: TRICE8_2(Id( 5774), "tst:TRICE8_2 %d %d\n",                   -1, -2);
  //break; case __LINE__: TRICE8_3(Id( 3757), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
  //break; case __LINE__: TRICE8_4(Id( 1007), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
  //break; case __LINE__: TRICE8_5(Id( 7585), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
  //break; case __LINE__: TRICE8_6(Id( 1339), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
  //break; case __LINE__: TRICE8_7(Id( 7427), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE8_8(Id( 7742), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
  //break; case __LINE__: TRICE8_1(Id( 1100), "tst:TRICE8_1 %d\n", 1);
  //break; case __LINE__: TRICE8_2(Id( 6230), "tst:TRICE8_2 %d %d\n", 1, 2);
  //break; case __LINE__: TRICE8_3(Id( 5507), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
  //break; case __LINE__: TRICE8_4(Id( 2586), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
  //break; case __LINE__: TRICE8_5(Id( 5057), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
  //break; case __LINE__: TRICE8_6(Id( 7265), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
  //break; case __LINE__: TRICE8_7(Id( 5228), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
  //break; case __LINE__: TRICE8_8(Id( 5845), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
  //break; case __LINE__: TRICE8_1(Id( 4911), "tst:TRICE8_1 %u\n", 201);
  //break; case __LINE__: TRICE8_2(Id( 3163), "tst:TRICE8_2 %u %u\n", 201, 202);
  //break; case __LINE__: TRICE8_3(Id( 5794), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
  //break; case __LINE__: TRICE8_4(Id( 2901), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
  //break; case __LINE__: TRICE8_5(Id( 4852), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
  //break; case __LINE__: TRICE8_6(Id( 7917), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
  //break; case __LINE__: TRICE8_7(Id( 2737), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
  //break; case __LINE__: TRICE8_8(Id( 4031), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
  //break; case __LINE__: TRICE8_8(Id( 1066), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);
    break; case __LINE__: TRICE( Id( 5850), "sig:Legacy TRICE16-64\n" );
    break; case __LINE__: TRICE16_1(Id( 6186), "tst:TRICE16_1 %d\n", -111);
    break; case __LINE__: TRICE16_2(Id( 3709), "tst:TRICE16_2 %d %d\n", -111, -222);
    break; case __LINE__: TRICE16_3(Id( 5091), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
    break; case __LINE__: TRICE16_4(Id( 1705), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
    break; case __LINE__: TRICE32_1(Id( 5759), "tst:TRICE32_1 %08x\n", 0x0123cafe);
    break; case __LINE__: TRICE32_1(Id( 7067), "tst:TRICE32_1 %d\n", -111);
    break; case __LINE__: TRICE32_2(Id( 1298), "tst:TRICE32_2 %x %x\n", -111, -222);
    break; case __LINE__: TRICE32_2(Id( 4232), "tst:TRICE32_2 %d %d\n", -111, -222);
    break; case __LINE__: TRICE32_3(Id( 7811), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
    break; case __LINE__: TRICE32_3(Id( 2310), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
    break; case __LINE__: TRICE32_4(Id( 5509), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
    break; case __LINE__: TRICE32_4(Id( 6491), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
    break; case __LINE__: TRICE64_1(Id( 2357), "tst:TRICE64_1 %d\n", -111);
    break; case __LINE__: TRICE64_2(Id( 1624), "tst:TRICE64_2 %d %d\n", -111, -222);
    break; case __LINE__: TRICE16_1(Id( 1215), "tst:TRICE16_1 %u\n", 60001);
    break; case __LINE__: TRICE16_2(Id( 3814), "tst:TRICE16_2 %u %u\n", 60001, 60002);
    break; case __LINE__: TRICE16_3(Id( 1733), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
    break; case __LINE__: TRICE16_4(Id( 4825), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
    break; case __LINE__: TRICE32_1(Id( 5750), "tst:TRICE32_1 %u\n", 4000000001);
    break; case __LINE__: TRICE32_2(Id( 4718), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
    break; case __LINE__: TRICE32_3(Id( 7121), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
    break; case __LINE__: TRICE32_4(Id( 7924), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
    break; case __LINE__: TRICE64_1(Id( 1438), "tst:TRICE64_1 %x\n", -1);
    break; case __LINE__: TRICE64_2(Id( 7178), "tst:TRICE64_2 %x %x\n", -1, -2);
    break; case __LINE__: TRICE64_1(Id( 4602), "tst:TRICE64_1 %u\n", -1);
    break; case __LINE__: TRICE64_2(Id( 1674), "tst:TRICE64_2 %u %u\n", -1, -2);
  //break; case __LINE__: TRICE( Id( 3215), "sig:colored single letters and several TRICE macros in one line\n" );
  //break; case __LINE__: TRICE(Id( 7578), "e:A");
  //break; case __LINE__: TRICE(Id( 1760), "w:B");
  //break; case __LINE__: TRICE(Id( 3295), "a:c");
  //break; case __LINE__: TRICE(Id( 7049), "wr:d");
  //break; case __LINE__: TRICE(Id( 7195), "rd:e\n");
  //break; case __LINE__: TRICE(Id( 7198), "diag:f");
  //break; case __LINE__: TRICE(Id( 6642), "d:G");
  //break; case __LINE__: TRICE(Id( 5604), "t:H");
  //break; case __LINE__: TRICE(Id( 5872), "time:i");
  //break; case __LINE__: TRICE(Id( 7513), "message:J");
  //break; case __LINE__: TRICE(Id( 3551), "dbg:k\n");
  //break; case __LINE__: TRICE(Id( 5992), "1");
  //break; case __LINE__: TRICE(Id( 7326), "2");
  //break; case __LINE__: TRICE(Id( 3028), "3");
  //break; case __LINE__: TRICE(Id( 1286), "4");
  //break; case __LINE__: TRICE(Id( 2442), "e:7");
  //break; case __LINE__: TRICE(Id( 1435), "m:12");
  //break; case __LINE__: TRICE(Id( 7371), "m:123\n");
  //break; case __LINE__: TRICE(Id( 5072), "e:A");TRICE(Id( 2858), "w:B");TRICE(Id( 3216), "a:c");
  //break; case __LINE__: TRICE(Id( 6050), "wr:d");TRICE(Id( 2486), "rd:e\n");//TRICE(Id( 4459), "diag:f");
/*
    break; case __LINE__: TRICE( Id( 2704), "sig:Runtime generated strings\n" );
    break; case __LINE__: {
                              s = "AAAAAAAAAAAA";
                              TRICE32( Id( 1250), "dbg:len=%u:", strlen(s) );
                              TRICE_S( Id( 4727), "sig:%s\n", s );
                          }
    break; case __LINE__: {
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
                          }
    break; case __LINE__: TRICE32( Id( 7524), "dbg:len=%u:", strlen(s) );
    break; case __LINE__: TRICE_S( Id( 3859), "sig:%s\n", s );
    break; case __LINE__: TRICE( Id( 1888), "sig:Runtime generated strings\n" );
    break; case __LINE__: {
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
                          }
    break; case __LINE__: TRICE32( Id( 3604), "dbg:len=%u: ", strlen(s) );
    break; case __LINE__: TRICE_S( Id( 6694), "sig:%s\n", s );
    break; case __LINE__: TRICE( Id( 3039), "sig:Runtime generated strings\n" );
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
    break; case __LINE__: TRICE32( Id( 2893), "dbg:len=%u: ", strlen(s) );
    break; case __LINE__: TRICE_S( Id( 7926), "sig:%s\n", s );
    break; case __LINE__: TRICE( Id( 5501), "sig:Runtime generated strings\n" );
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
    break; case __LINE__: TRICE32( Id( 6407), "dbg:len=%u: ", strlen(s) );
    break; case __LINE__: TRICE_S( Id( 4560), "sig:%s\n", s );
    break; case __LINE__: { // https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/
                              int i = 15;
                              TRICE( Id( 1145), "sig:Integer (indent, base, sign)\n" );
                              TRICE( Id( 4395), "rd: 15 	%d 	Base 10\n", i );
                              TRICE( Id( 2176), "rd: +15 	%+d 	Always show sign\n", i );
                              TRICE( Id( 4515), "rd:   15 	%4d 	Pad with spaces (width 4, right justified)\n", i );
                              TRICE( Id( 4873), "rd: 15   	%-4d 	Pad with spaces (width 4, left justified)\n", i );
                              TRICE( Id( 2317), "rd: 0015 	%04d 	Pad with zeroes (width 4)\n", i );
                              TRICE( Id( 2747), "rd: 1111 	%b 	Base 2\n", i );
                              TRICE( Id( 4721), "rd: 17 	%o 	Base 8\n", i );
                              TRICE( Id( 3272), "rd: f 	%x 	Base 16, lowercase\n", i );
                              TRICE( Id( 6890), "rd: F 	%X 	Base 16, uppercase\n", i );
                              TRICE( Id( 3393), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", i );
                          }

    break; case __LINE__: {
                              char a = 'A';
                              TRICE( Id( 5272), "sig:Character (quoted, Unicode)\n" );
                              TRICE( Id( 4141), "rd: A 	%c 	Character\n", a );
                              TRICE( Id( 3347), "rd: 'A' 	%q 	Quoted character\n", a );
                              TRICE( Id( 4503), "rd: U+0041 	%U 	Unicode\n", a );// todo
                              TRICE( Id( 3190), "rd: U+0041 'A' 	%#U 	Unicode with character\n", a );// todo
                              TRICE( Id( 7022), "sig:Boolean (true/false)\n" );
                              TRICE( Id( 2469), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 );
                              TRICE( Id( 6962), "sig:Pointer (hex)\n" );
                          }

    break; case __LINE__: TRICE8 ( Id( 1531), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );
    break; case __LINE__: TRICE16( Id( 3400), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );
    break; case __LINE__: TRICE32( Id( 4152), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );
    break; case __LINE__: TRICE64( Id( 2932), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 );
    break; case __LINE__: TRICE  ( Id( 4223), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);
    break; case __LINE__: {
                                float f = (float)123.456; 
                                TRICE( Id( 3894), "sig:Float (indent, precision, scientific notation)\n" );
                                TRICE( Id( 6604), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(f) );
                                TRICE( Id( 4732), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(f) );
                                TRICE( Id( 1355), "rd: 123.46	 		%.2f		%%.2f Default width, precision 2\n", aFloat(f) );
                                TRICE( Id( 7401), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(f) );
                                TRICE( Id( 5853), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(f) );
                          }
    break; case __LINE__: {
                                double d = 123.456;
                                TRICE( Id( 7733), "sig:Double (indent, precision, scientific notation)\n" );
                                TRICE64( Id( 5436), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(d) );
                                TRICE64( Id( 7633), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(d) );
                                TRICE64( Id( 5546), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(d) );
                                TRICE64( Id( 5887), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(d) );
                                TRICE64( Id( 3932), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(d) );
                          }
    break; case __LINE__: {
                              s = "café";
                              TRICE( Id( 6233), "sig:String or byte slice (quote, indent, hex)\n" );
                              TRICE_S( Id( 4646), "rd: café 			%s		Plain string\n", s );
                              TRICE_S( Id( 1585), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );
                              TRICE_S( Id( 3587), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );
                              TRICE_S( Id( 1068), "rd: \"café\" 		%q	 	Quoted string\n", s );
                              TRICE_S( Id( 3496), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );
                              TRICE_S( Id( 1543), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );
                          }
*/
      //break; case __LINE__: TRICE( Id( 2879), "sig:TRICE8 with variable param count 1 to 12\n" );
      //break; case __LINE__: TRICE8(Id( 5590), "tst:TRICE8 %d\n", -1 );
      //break; case __LINE__: TRICE8(Id( 1883), "tst:TRICE8 %d %d\n", -1, -2 );
      //break; case __LINE__: TRICE8(Id( 6651), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
      //break; case __LINE__: TRICE8(Id( 4738), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
      //break; case __LINE__: TRICE8(Id( 1391), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
      //break; case __LINE__: TRICE8(Id( 1319), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
      //break; case __LINE__: TRICE8(Id( 3797), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
      //break; case __LINE__: TRICE8(Id( 2405), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //break; case __LINE__: TRICE8(Id( 2548), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //break; case __LINE__: TRICE8(Id( 1707), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
      //break; case __LINE__: TRICE8(Id( 5855), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
      //break; case __LINE__: TRICE8(Id( 2837), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
      //break; case __LINE__: TRICE8(Id( 6105), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
      //break; case __LINE__: TRICE8_1 (Id( 2730), "tst:TRICE8_1  %d\n", -1 );
      //break; case __LINE__: TRICE8_2 (Id( 7130), "tst:TRICE8_2  %d %d\n", -1, -2 );
      //break; case __LINE__: TRICE8_3 (Id( 2492), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
      //break; case __LINE__: TRICE8_4 (Id( 7999), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
      //break; case __LINE__: TRICE8_5 (Id( 7482), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
      //break; case __LINE__: TRICE8_6 (Id( 7854), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
      //break; case __LINE__: TRICE8_7 (Id( 4620), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
      //break; case __LINE__: TRICE8_8 (Id( 1779), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //break; case __LINE__: TRICE8_9 (Id( 7852), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //break; case __LINE__: TRICE8_10(Id( 3998), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
      //break; case __LINE__: TRICE8_11(Id( 7851), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
      //break; case __LINE__: TRICE8_12(Id( 3237), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
      //break; case __LINE__: TRICE8  (Id( 4146), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
      //break; case __LINE__: TRICE8_9(Id( 4661), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
      //break; case __LINE__: TRICE( Id( 4156), "sig:TRICE16 with variable param count 1 to 12\n" );
      //break; case __LINE__: TRICE16(Id( 3331), "tst:TRICE16 %d\n", -1 );
      //break; case __LINE__: TRICE16(Id( 1903), "tst:TRICE16 %d %d\n", -1, -2 );
      //break; case __LINE__: TRICE16(Id( 1726), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
      //break; case __LINE__: TRICE16(Id( 3568), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
      //break; case __LINE__: TRICE16(Id( 2971), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
      //break; case __LINE__: TRICE16(Id( 5210), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
      //break; case __LINE__: TRICE16(Id( 7141), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
      //break; case __LINE__: TRICE16(Id( 6274), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //break; case __LINE__: TRICE16(Id( 7611), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //break; case __LINE__: TRICE16(Id( 3586), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
      //break; case __LINE__: TRICE16(Id( 5566), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
      //break; case __LINE__: TRICE16(Id( 6209), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
      //break; case __LINE__: TRICE16(Id( 6771), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
      //break; case __LINE__: TRICE16_1 (Id( 2164), "tst:TRICE16_1  %d\n", -1 );
      //break; case __LINE__: TRICE16_2 (Id( 1185), "tst:TRICE16_2  %d %d\n", -1, -2 );
      //break; case __LINE__: TRICE16_3 (Id( 4163), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
      //break; case __LINE__: TRICE16_4 (Id( 5339), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
      //break; case __LINE__: TRICE16_5 (Id( 7656), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
      //break; case __LINE__: TRICE16_6 (Id( 1091), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
      //break; case __LINE__: TRICE16_7 (Id( 7824), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
      //break; case __LINE__: TRICE16_8 (Id( 2048), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //break; case __LINE__: TRICE16_9 (Id( 6513), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //break; case __LINE__: TRICE16_10(Id( 1512), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
      //break; case __LINE__: TRICE16_11(Id( 1056), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
      //break; case __LINE__: TRICE16_12(Id( 5018), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
      //break; case __LINE__: TRICE16_9(Id( 7757), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        break; case __LINE__: TRICE( Id( 5378), "sig:TRICE32 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE32(Id( 7943), "tst:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32(Id( 5162), "tst:TRICE32 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32(Id( 6141), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32(Id( 3382), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32(Id( 3208), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32(Id( 7116), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32(Id( 2617), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32(Id( 7940), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32(Id( 6298), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32(Id( 1395), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32(Id( 7106), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32(Id( 4753), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32(Id( 7572), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_1 (Id( 3459), "tst:TRICE32_1  %d\n", -1 );
        break; case __LINE__: TRICE32_2 (Id( 3199), "tst:TRICE32_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32_3 (Id( 5847), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32_4 (Id( 5046), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32_5 (Id( 2327), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32_6 (Id( 5960), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32_7 (Id( 2864), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32_8 (Id( 5617), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32_9 (Id( 6763), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32_10(Id( 5476), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32_11(Id( 6477), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32_12(Id( 5365), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32   (Id( 6861), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_12(Id( 5433), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE( Id( 7056), "sig:TRICE64 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE64(Id( 2239), "tst:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64(Id( 7687), "tst:TRICE64 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64(Id( 6659), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64(Id( 5908), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64(Id( 4074), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64(Id( 6045), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64(Id( 4499), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64(Id( 4111), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64(Id( 6937), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64(Id( 5581), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64(Id( 1274), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64(Id( 5160), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64(Id( 3750), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break; case __LINE__: TRICE64_1 (Id( 5607), "tst:TRICE64_1  %d\n", -1 );
        break; case __LINE__: TRICE64_2 (Id( 4168), "tst:TRICE64_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64_3 (Id( 3373), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64_4 (Id( 2366), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64_5 (Id( 6911), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64_6 (Id( 2378), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64_7 (Id( 4079), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64_8 (Id( 2346), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64_9 (Id( 4797), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64_10(Id( 1675), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64_11(Id( 1906), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64_12(Id( 3166), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64_12(Id( 7232), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break; case __LINE__: TRICE( Id( 6600), "att:positive and negative float in format variants\n");
        break; case __LINE__: TRICE( Id( 7602), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 7451), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 4764), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 6718), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 2235), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 6012), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 2885), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 6405), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 7230), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 2866), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 6323), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 3633), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 2860), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 6343), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 6126), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 4185), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 6814), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 5532), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 2395), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 5920), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 5990), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 3165), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 6924), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 5788), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 2719), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 1730), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 7366), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 4524), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 1242), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 5239), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 2562), "att:positive float & double in variants\n");
        break; case __LINE__: TRICE32_1( Id( 1207), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( Id( 6317), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( Id( 2703), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( Id( 6427), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( Id( 1634), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( Id( 1742), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE32( Id( 6112), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32( Id( 6134), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32( Id( 1782), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32( Id( 7825), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32( Id( 3180), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32( Id( 3552), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 7711), "rd:TRICE float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 5032), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 3045), "rd:TRICE float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 7221), "rd:TRICE float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 5201), "rd:TRICE float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE( Id( 5371), "rd:TRICE float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE64( Id( 1380), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( Id( 7114), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( Id( 4599), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( Id( 1144), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( Id( 2564), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( Id( 5423), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( Id( 1283), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( Id( 7299), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( Id( 5620), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( Id( 1668), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( Id( 4901), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( Id( 3689), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE( Id( 1214), "att:negative float & double\n" );
        break; case __LINE__: TRICE( Id( 7788), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 2804), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 2322), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 2587), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 7697), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
        break; case __LINE__: TRICE( Id( 4845), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
        break; case __LINE__: TRICE64( Id( 4641), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( Id( 4274), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( Id( 3410), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( Id( 7316), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( Id( 4101), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( Id( 4291), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE( Id( 1018), "att:formatted float & double\n" );
        break; case __LINE__: TRICE32( Id( 6467), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
        break; case __LINE__: TRICE32( Id( 1522), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE32( Id( 1013), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
        break; case __LINE__: TRICE32( Id( 2384), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
        break; case __LINE__: TRICE32( Id( 1713), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
        break; case __LINE__: TRICE32( Id( 6582), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
        break; case __LINE__: TRICE64( Id( 1162), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( Id( 1776), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( Id( 6276), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( Id( 6374), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( Id( 6010), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( Id( 6703), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE( Id( 5028), "att:mixed int & float & double & bits\n" );
      //  break; case __LINE__: TRICE32( Id( 5867), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(x), aFloat(x), aFloat(x), aFloat(x) );
      //  break; case __LINE__: TRICE32( Id( 1369), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(x), aFloat(x), aFloat(x), aFloat(x) );
      //  break; case __LINE__: TRICE32( Id( 3454), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              x,  aFloat(x), aFloat(x), aFloat(x) );
      //  break; case __LINE__: TRICE32( Id( 3967), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(-x), aFloat(-x), aFloat(-x), aFloat(-x) );
      //  break; case __LINE__: TRICE32( Id( 7506), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(-x), aFloat(-x), aFloat(-x), aFloat(-x) );
      //  break; case __LINE__: TRICE32( Id( 4804), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              -x,  aFloat(-x), aFloat(-x), aFloat(-x) );
      //  break; case __LINE__: TRICE64( Id( 4150), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(y), aDouble(y), aDouble(y), aDouble(y) );
      //  break; case __LINE__: TRICE64( Id( 1427), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(y), aDouble(y), aDouble(y), aDouble(y) );
      //  break; case __LINE__: TRICE64( Id( 4811), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               y,  aDouble(y), aDouble(y), aDouble(y) );
      //  break; case __LINE__: TRICE64( Id( 2409), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) );
      //  break; case __LINE__: TRICE64( Id( 7884), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) );
      //  break; case __LINE__: TRICE64( Id( 7235), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               -y,  aDouble(-y), aDouble(-y), aDouble(-y) );
      //break; case __LINE__: TRICE( Id( 6355), "att:Various single arguments\n" );
      //break; case __LINE__: TRICE8( Id( 5174), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
      //break; case __LINE__: TRICE8( Id( 5446), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
      //break; case __LINE__: TRICE8( Id( 6921), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
      //break; case __LINE__: TRICE8( Id( 5946), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
      //break; case __LINE__: TRICE8( Id( 2275), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
      //break; case __LINE__: TRICE8( Id( 3212), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
      //break; case __LINE__: TRICE8( Id( 2935), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
      //break; case __LINE__: TRICE8( Id( 5708), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
      //break; case __LINE__: TRICE8( Id( 1413), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
      //break; case __LINE__: TRICE16_1( Id( 1719), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
      //break; case __LINE__: TRICE16_1( Id( 5733), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
      //break; case __LINE__: TRICE16_1( Id( 1119), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
      //break; case __LINE__: TRICE16_1( Id( 4775), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
      //break; case __LINE__: TRICE16_1( Id( 6806), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
      //break; case __LINE__: TRICE16_1( Id( 6412), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
      //break; case __LINE__: TRICE16_1( Id( 2332), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
        break; case __LINE__: TRICE( Id( 2418), "rd:TRICE line %t (%%t -1)\n", 0 );
        break; case __LINE__: TRICE( Id( 3853), "rd:TRICE line %t (%%t -1)\n", 2 );
        break; case __LINE__: TRICE( Id( 3059), "rd:TRICE line %u (%%u)\n", -1 );
        break; case __LINE__: TRICE( Id( 7441), "rd:TRICE line %b (%%b)\n", -2 );
        break; case __LINE__: TRICE( Id( 3699), "rd:TRICE line %o (%%o)\n", -3 );
        break; case __LINE__: TRICE( Id( 7335), "rd:TRICE line %x (%%x)\n", -4 );
        break; case __LINE__: TRICE( Id( 4998), "rd:TRICE line %d (%%d)\n", -5 );
        break; case __LINE__: TRICE_N( Id( 6267), "msg:%s\n", A, 0 );
        break; case __LINE__: TRICE_N( Id( 5767), "msg:%s\n", A, 1 );
        break; case __LINE__: TRICE_N( Id( 3077), "msg:%s\n", A, 2 );
        break; case __LINE__: TRICE_N( Id( 6160), "msg:%s\n", A, 3 );
        break; case __LINE__: TRICE_N( Id( 6481), "msg:%s\n", A, 4 );
        break; case __LINE__: TRICE_N( Id( 1349), "msg:%s\n", A, 5 );
        break; case __LINE__: TRICE_N( Id( 7761), "msg:%s\n", A, 6 );
        break; case __LINE__: TRICE_N( Id( 7698), "msg:%s\n", A, 7 );
        break; case __LINE__: TRICE_N( Id( 4433), "msg:%s\n", A, 8 );
        break; case __LINE__: TRICE_N( Id( 3000), "msg:%s\n", A, 9 );
        break; case __LINE__: TRICE_N( Id( 7050), "msg:%s\n", A, 10 );
        break; case __LINE__: TRICE_N( Id( 6727), "msg:%s\n", A, 11 );
        break; case __LINE__: TRICE_N( Id( 7329), "msg:%s\n", A, 12 );
        break; case __LINE__: TRICE_N( Id( 1096), "msg:%s\n", A, 13 );
        break; case __LINE__: TRICE_N( Id( 1224), "msg:%s\n", A, 14 );
        break; case __LINE__: TRICE_N( Id( 5633), "msg:%s\n", A, 15 );
        break; case __LINE__: TRICE_N( Id( 1804), "msg:%s\n", A, 16 );
        break; case __LINE__: TRICE_N( Id( 2613), "msg:%s\n", A, 17 );
        break; case __LINE__: TRICE_N( Id( 3116), "msg:%s\n", A, 18 );
        break; case __LINE__: TRICE_N( Id( 2484), "msg:%s\n", A, 19 );
        break; case __LINE__: TRICE_N( Id( 4046), "msg:%s\n", A, 120 );
        break; case __LINE__: TRICE_N( Id( 1889), "msg:%s\n", A, 121 );
        break; case __LINE__: TRICE_N( Id( 2815), "msg:%s\n", A, 122 );
        break; case __LINE__: TRICE_N( Id( 3121), "msg:%s\n", A, 123 );
        break; case __LINE__: TRICE_N( Id( 5491), "msg:%s\n", A, 124 );
        break; case __LINE__: TRICE_N( Id( 3949), "msg:%s\n", A, 125 );
        break; case __LINE__: TRICE_N( Id( 7162), "msg:%s\n", A, 126 );
        break; case __LINE__: TRICE_N( Id( 2898), "msg:%s\n", A, 127 );
        break; case __LINE__: TRICE_N( Id( 1110), "msg:%s\n", A, 128 );
        break; case __LINE__: TRICE_N( Id( 7370), "msg:%s\n", A, 129 );
        break; case __LINE__: exampleOfManualJSONencoding(); // exp: what?
        break; case __LINE__: exampleOfBuffersAndFunctions();
        break; case __LINE__: exampleOfManualSerialization();
        break; case __LINE__: 
//  #ifdef XTEA_ENCRYPT_KEY
//          TRICE0 (Id( 4367), "--------------------------------------------------\n" );
//          {
//              uint32_t by[8] = {1,2,3,4,5,6};
//              int count = (sizeof(by) + 7) & ~7; // only multiple of 8 encryptable
//              TRICE32(  Id( 7553), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              TRICE32( Id( 5825), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
//              XTEAEncrypt(by, count>>2);
//              TRICE32( Id( 5486), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
//              TRICE32(  Id( 2849), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              #ifdef XTEA_DECRYPT
//              TRICE16_1( Id( 7182), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
//              XTEADecrypt(by, count>>2);
//              TRICE16_1( Id( 7102), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
//              #endif
//              TRICE32(  Id( 4093), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//          }
//          TRICE0 (Id( 2388), "--------------------------------------------------\n" );
//  #endif
        break; case __LINE__: showTriceDepth();
    }
}






//! SCOPY is a helper macro for struct serialization.
#define SCOPY( element ) do{ char* n = #element; int size = sizeof( src->element ); memcpy( p, &(src->element), size ); p += size; TRICE_S( Id(10433), "rd:sizeof(%8s)", n ); TRICE( Id(14289), " = %d\n", size);}while(0);

    //! DCOPY is a helper macro for struct deserialization.
#define DCOPY( element ) do{ char* n = #element; int size = sizeof( dst->element ); memcpy( &(dst->element), p, size ); p += size; TRICE_S( Id(13474), "rd:sizeof(%8s)", n ); TRICE( Id(13800), " = %d\n", size);}while(0);


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
    TRICE ( Id(11442), "inf: Tryout tx struct:" );
    TRICE_B( Id(11284), " %02x ", &tx, sizeof(tx) );
    TRICE( Id(15779), "\n" );
    
    TRICE ( Id(14357), "inf: Tryout buffer:" );
    TRICE_B( Id(13606), " %02x ", dst, len ); //lint !e670
    TRICE( Id(15046), "\n" );

    src = dst; // "data transfer"
    
    len = deserializeTryout( &rx, src );
    TRICE ( Id(12502), "inf: Tryout rx struct:" );
    TRICE_B( Id(13829), " %02x ", &rx, sizeof(rx) );
    TRICE( Id(14071), "\n" );

    TRICE( Id(14648), "inf:sizeOf(Trypout) = %d, buffer length = %d\n", sizeof(tx), len );
    TRICE8_F( Id(14705), "info:TryoutStructFunction", &tx, sizeof(tx) );
    TRICE8_F( Id(14395), "info:TryoutBufferFunction", dst, len ); //lint !e670
}

static void exampleOfManualJSONencoding(void){
    typedef  struct {
    int Apple, Birn;
    float Fish;
    } Ex_t;
    Ex_t Ex = { -1, 2, (float)2.781 };
    TRICE( Id(13149), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish) );
}

static void exampleOfBuffersAndFunctions(void){
    static int8_t   b8[24] = { 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11, 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11 };
    static int16_t b16[] = { 0, -1, -2, 0x3344 };
    static int32_t b32[] = { 0, -1, -2, 0x33445555};
    static int64_t b64[4] = { 0, -1, -2, 0x3344555566666666 };
    char* s = "abcde 12345";
    uint32_t len;

    TRICE_S( Id(11119), "msg:With TRICE_S:%s\n", s );
    len = strlen(s);
    TRICE_N( Id(14748), "sig:With TRICE_N:%s\n", s, len );
    TRICE32( Id(15560), "att:len=%u:With TRICE_B:\n", len);
    TRICE_B( Id(15535), "  %02x", s, len );
    TRICE( Id(11168), "\n" );
    TRICE_B( Id(12385), "%4d", s, len );
    TRICE( Id(12939), "\n" );

    TRICE( Id(10771), "notice:TRICE_B example: " );
    TRICE_B( Id(13006), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );
    TRICE( Id(11618), "\n" );
    TRICE( Id(13884), "notice:TRICE8_B example:" );
    TRICE8_B( Id(14956), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );
    TRICE( Id(14378), "\n" );
    TRICE( Id(15376), "notice:TRICE16_B example:" );
    TRICE16_B( Id(13926), "  %04x", b16, sizeof(b16)/sizeof(int16_t) );
    TRICE( Id(12037), "\n" );
    TRICE( Id(15001), "notice:TRICE32_B example:" );
    TRICE32_B( Id(13642), "  %08x", b32, sizeof(b32)/sizeof(int32_t) );
    TRICE( Id(11092), "\n" );
    TRICE( Id(10846), "notice:TRICE64_B example:" );
    TRICE64_B( Id(10243), "  %016x", b64, sizeof(b64)/sizeof(int64_t) );
    TRICE( Id(10936), "\n" );    

    TRICE( Id(11916), "notice:TRICE_F example: " );
    TRICE_F( Id(14245), "info:FunctionNameW",   b8,  sizeof(b8) /sizeof(int8_t) );
    //shown as: TRICE_F example: FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
    
    TRICE( Id(15171), "notice:TRICE8_F example:" );
    TRICE8_F(  Id(12053), "info:FunctionNameW",   b8,  sizeof(b8) /sizeof(int8_t) );
    //shown as: TRICE8_F example:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
    
    TRICE( Id(10625), "notice:TRICE16_F example:" );
    TRICE16_F( Id(12132), "sig:FunctionNameX",    b16, sizeof(b16)/sizeof(int16_t) );
    //shown as: TRICE16_F example:FunctionNameX(0000)(ffff)(fffe)(3344) 

    TRICE( Id(12279), "notice:TRICE32_F example:" );
    TRICE32_F( Id(11436), "diag:FunctionNameY",   b32, sizeof(b32)/sizeof(int32_t) );
    //shown as: TRICE32_F example:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)

    TRICE( Id(14838), "notice:TRICE64_F example:" );
    TRICE64_F( Id(15889), "fatal:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );
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
