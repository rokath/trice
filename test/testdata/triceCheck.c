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


//! SCOPY is a helper macro for struct serialization.
#define SCOPY( element ) do{ char* n = #element; int size = sizeof( src->element ); memcpy( p, &(src->element), size ); p += size; TRICE_S( Id(10433), "rd:sizeof(%8s)", n ); TRICE( Id(14289), " = %d\n", size);}while(0);

    //! DCOPY is a helper macro for struct deserialization.
#define DCOPY( element ) do{ char* n = #element; int size = sizeof( dst->element ); memcpy( &(dst->element), p, size ); p += size; TRICE_S( Id(13474), "rd:sizeof(%8s)", n ); TRICE( Id(13800), " = %d\n", size);}while(0);


typedef struct{
    float x;
    float y;
    uint8_t rgb[3];
    //float z; // it seems, that the compiler does not align this with -o3 & time !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
} Point_t; //!< Point_t is small struct type.

static int serializePoint( char* dst, Point_t const * src ){
    char * p = dst;

    SCOPY( x )
    SCOPY( y )
    SCOPY( rgb )

    return p - dst;
}

static int deserializePoint( Point_t * const dst, char const * src ){
    char const * p = src;

    DCOPY( x )
    DCOPY( y )
    DCOPY( rgb )
    
    return p - src;
}

typedef struct{
    float z;
    uint16_t u;
      int8_t s;
    char* addr;
    uint32_t x;
     int32_t y;
    char names[3][5];
    Point_t point[2];
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
    p += serializePoint( p, src->point );
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
    p += deserializePoint( dst->point, p );
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

    tx.point[0].x = 2.22;
    tx.point[0].y = -3.33;
    tx.point[0].rgb[0] = 0x44;
    tx.point[0].rgb[0] = 0x66;
    tx.point[0].rgb[0] = 0x88;
    
    tx.point[1].x = -66.66;
    tx.point[1].y = +5.5555;
    tx.point[1].rgb[0] = 0xee;
    tx.point[1].rgb[0] = 0xaa;
    tx.point[1].rgb[0] = 0xbb;

    tx.bitmask = 0xAAAA55550000FFFF;
    //
    ////////////////////////////////////////////////////////

    len = serializeTryout( dst, &tx );
    TRICE ( Id( 7232), "inf: Tryout tx struct:" );
    TRICE8_B( Id( 4250), " %02x ", &tx, sizeof(tx) );
    TRICE( Id( 4642), "\n" );

    TRICE ( Id( 3276), "inf: Tryout buffer:" );
    TRICE8_B( Id( 7454), " %02x ", dst, len ); //lint !e670
    TRICE( Id( 2544), "\n" );

    src = dst; // "data transfer"

    len = deserializeTryout( &rx, src );
    TRICE ( Id( 2910), "inf: Tryout rx struct:" );
    TRICE8_B( Id( 5440), " %02x ", &rx, sizeof(rx) );
    TRICE( Id( 6338), "\n" );

    TRICE( Id( 2135), "inf:sizeOf(Trypout) = %d, buffer length = %d\n", sizeof(tx), len );
    TRICE8_F( Id( 5049), "info:TryoutStructFunction", &tx, sizeof(tx) );
    TRICE8_F( Id( 5639), "info:TryoutBufferFunction", dst, len ); //lint !e670
}

static void exampleOfManualJSONencoding(void){
    typedef  struct {
    int Apple, Birn;
    float Fish;
    } Ex_t;
    Ex_t Ex = { -1, 2, (float)2.781 };
    TRICE( Id(13149), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish) );
}

static void showTriceDepth( void ){
    #if TRICE_MODE == TRICE_DOUBLE_BUFFER
    {
        uint16_t triceDepth = TriceDepth();
        uint16_t triceDepthMax = TriceDepthMax();
        TRice16( iD( 7794), "MSG: ?� STOP  TriceDepth = %d, TriceDepthMax = %d + %4u of %d\n", triceDepth, TRICE_DATA_OFFSET, triceDepthMax-TRICE_DATA_OFFSET, TRICE_HALF_BUFFER_SIZE );
    } 
    #endif
    #if TRICE_MODE == TRICE_STREAM_BUFFER
        TRice( iD( 6539), "MSG:triceFifoDepthMax = %d of max %d, triceStreamBufferDepthMax = %d of max %d\n", triceFifoDepthMax, TRICE_FIFO_ELEMENTS, triceStreamBufferDepthMax, TRICE_BUFFER_SIZE );
    #endif
}

void TriceLogDepthMax( void ){
    #if TRICE_MODE == TRICE_DOUBLE_BUFFER
        size_t tdm = TriceDepthMax();
        if( tdm <= TRICE_HALF_BUFFER_SIZE ){
            TRice16( iD( 4784), "diag:TriceDepthMax =%4u of %d\n", tdm, TRICE_HALF_BUFFER_SIZE );
        }else{
            TRice16( iD( 5607), "err:TriceDepthMax =%4u of %d (overflow!)\n", tdm, TRICE_HALF_BUFFER_SIZE );
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
    char* s = "AAAAAAAAAAAA";
    float  x = (float)1089.6082763671875; // 0x44883377
    double y = 518.0547492508867; // 0x4080307020601050  
    static int8_t b8[24] = { 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11, 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11 };
    static int16_t b16[] = { 0, -1, -2, 0x3344 };
    static int32_t b32[] = { 0, -1, -2, 0x33445555};
    static int64_t b64[4] = { 0, -1, -2, 0x3344555566666666 };
    char* sABCDE = "abcde 12345";
    uint32_t lenABCDE = strlen(sABCDE);

    #ifdef TRICE_CGO
    char* A = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    #endif // #ifdef TRICE_CGO

    switch( n ){
        default:

        // normal use cases (3 macros)
      //break; case __LINE__: trice      ( iD( 1167), "value=%d\n", -2  ); //exp: time:        value=-2
      //break; case __LINE__: Trice      ( iD( 5963), "value=%d\n", -2  ); //exp: time:    1616value=-2
      //break; case __LINE__: TRice      ( iD( 4209), "value=%d\n", -2  ); //exp: time:32323232value=-2
      //break; case __LINE__: trice_1    ( iD( 3105), "value=%d\n", -2  ); //exp: time:        value=-2
      //break; case __LINE__: Trice_1    ( iD( 7103), "value=%d\n", -2  ); //exp: time:    1616value=-2
      //break; case __LINE__: TRice_1    ( iD( 7476), "value=%d\n", -2  ); //exp: time:32323232value=-2

        break; case __LINE__: exampleOfManualSerialization();

        break; case __LINE__: trice8   ( iD( 5574),         "value=%x\n", 0x55 );                           //exp: time:        value=55
        break; case __LINE__: trice8   ( iD( 4092),  "value=%x\n", 0x88 );                           //exp: time:        value=88

        break; case __LINE__: TRICE( ID( 1822), "att:mixed int & float & double & bits\n" );
        break; case __LINE__: TRICE32( ID( 2568), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(x), aFloat(x), aFloat(x), aFloat(x) );           //exp: time:32323232rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID( 6946), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(x), aFloat(x), aFloat(x), aFloat(x) );           //exp: time:32323232rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID( 2543), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              x,  aFloat(x), aFloat(x), aFloat(x) );           //exp: time:32323232rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID( 2447), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(-x), aFloat(-x), aFloat(-x), aFloat(-x) );       //exp: time:32323232rd:TRICE32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
        break; case __LINE__: TRICE32( ID( 4113), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(-x), aFloat(-x), aFloat(-x), aFloat(-x) );       //exp: time:32323232rd:TRICE32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
    //  break; case __LINE__: TRICE32( ID( 7407), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              -x,  aFloat(-x), aFloat(-x), aFloat(-x) );       //exp: time:32323232rd:TRICE32 int 1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                        
        break; case __LINE__: TRICE64( ID( 5248), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(y), aDouble(y), aDouble(y), aDouble(y) );     //exp: time:32323232rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID( 2554), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(y), aDouble(y), aDouble(y), aDouble(y) );     //exp: time:32323232rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID( 5920), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               y,  aDouble(y), aDouble(y), aDouble(y) );     //exp: time:32323232rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID( 5529), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:32323232rd:TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
        break; case __LINE__: TRICE64( ID( 2646), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:32323232rd:TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
    //  break; case __LINE__: TRICE64( ID( 7117), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               -y,  aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:32323232rd:TRICE64 int 518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000 

        break; case __LINE__: TRICE_S( ID( 5017), "msg:With TRICE_S:%s\n", sABCDE );                           //exp: time:32323232msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N( ID( 6196), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );                 //exp: time:32323232sig:With TRICE_N:abcde 12345
        break; case __LINE__: TRICE8_B( ID( 2301), "  %02x", sABCDE, lenABCDE );                               //exp: time:32323232  61  62  63  64  65  20  31  32  33  34  35
        break; case __LINE__: TRICE( ID( 6318), "\n" );
        break; case __LINE__: TRICE8_B( ID( 7925), "%4d", sABCDE, lenABCDE );                                  //exp: time:32323232  97  98  99 100 101  32  49  50  51  52  53
        break; case __LINE__: TRICE( ID( 3124), "\n" );
        break; case __LINE__: TRICE8_B( ID( 1292), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );                  //exp: time:32323232  00  ff  fe  33  04  05  06  07  08  09  0a  0b  00  ff  fe  33  04  05  06  07  08  09  0a  0b
        break; case __LINE__: TRICE( ID( 1176), "\n" );
        break; case __LINE__: TRICE16_B( ID( 6863), "  %04x", b16, sizeof(b16)/sizeof(int16_t) );              //exp: time:32323232  0000  ffff  fffe  3344
        break; case __LINE__: TRICE( ID( 4629), "\n" );
        break; case __LINE__: TRICE32_B( ID( 2192), "  %08x", b32, sizeof(b32)/sizeof(int32_t) );              //exp: time:32323232  00000000  ffffffff  fffffffe  33445555
        break; case __LINE__: TRICE( ID( 6438), "\n" );
        break; case __LINE__: TRICE64_B( ID( 4014), "  %016x", b64, sizeof(b64)/sizeof(int64_t) );             //exp: time:32323232  0000000000000000  ffffffffffffffff  fffffffffffffffe  3344555566666666
        break; case __LINE__: TRICE( ID( 5319), "\n" );
        break; case __LINE__: TRICE8_F( ID( 7728),  "info:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );   //exp: time:32323232info:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: TRICE16_F( ID( 5477), "info:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:32323232info:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: TRICE32_F( ID( 3950), "info:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:32323232info:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: TRICE64_F( ID( 2069), "info:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:32323232info:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: exampleOfManualJSONencoding(); // exp: time:    1616att:MyStructEvaluationFunction(json:ExA{Apple:-1, Birn:2, Fisch:2.781000}

        break; case __LINE__: TRICE( Id( 3885), "MSG:1/11 = %g\n", aFloat( 1.0/11 ) ); //exp: time:    1616MSG:1/11 = 0.09090909
        break; case __LINE__: { //exp: time:32323232msg:x = 5.934 = 5.934, 5.934
                              float a = (float)5.934;
                              float b = a + ((a > 0) ? 0.0005f : -0.0005f);
                              int c = (int)b;
                              int d = (int)(b * 1000) % 1000;
                              int e = (int)(1000 * (float)(a - c)); 
                              TRICE( ID( 7118), "msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e ); 
                              }

        break; case __LINE__: TRICE( id( 5606), "dbg:Hi!\n" );                       // exp: time:        dbg:Hi!    
        break; case __LINE__: TRICE( id( 2896), "dbg:\tHi!\n" );                     
        break; case __LINE__: TRICE( id( 4017), "dbg:\t\tHi!\n" );                   
        break; case __LINE__: TRICE( id( 3650), "dbg:\t\t\tHi!\n" );                 
        break; case __LINE__: TRICE( id( 5841), "dbg:\\tHi!\n" );                    
        break; case __LINE__: TRICE( Id( 1578), "dbg:\aHi!\n" ); // sound!
        break; case __LINE__: TRICE( Id( 7678), "dbg:\a\aHi!\n" ); // sound!
        break; case __LINE__: TRICE( Id( 1107), "dbg:\a\a\aHi!\n" ); // sound!
        break; case __LINE__: TRICE( Id( 2752), "dbg:\\aHi!\n" );
        break; case __LINE__: TRICE( Id( 7203), "err:ERROR!\a\n" ); // sound!

        break; case __LINE__: TRICE_S( ID( 4456), "sig:TRICE_S=%s\n", s );            // exp: time:32323232sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N( ID( 7893), "sig:TRICE_N=%s\n", s, strlen(s) ); // exp: time:32323232sig:TRICE_N=AAAAAAAAAAAA

        break; case __LINE__: TRICE  ( ID( 5337), "rd:%E (%%E)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE  ( ID( 1013), "rd:%F (%%F)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-555555584.000000 (%F)
        break; case __LINE__: TRICE  ( ID( 4082), "rd:%G (%%G)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556E+08 (%G)
        break; case __LINE__: TRICE64( ID( 1865), "rd:%E (%%E)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE64( ID( 4161), "rd:%F (%%F)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-555555555.555556 (%F)
        break; case __LINE__: TRICE64( ID( 4764), "rd:%G (%%G)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-5.555555555555556E+08 (%G)
        
        break; case __LINE__: TRICE  ( ID( 5352), "rd:%e (%%e)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE  ( ID( 3770), "rd:%f (%%f)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-555555584.000000 (%f)
        break; case __LINE__: TRICE  ( ID( 4488), "rd:%g (%%g)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556e+08 (%g)
        break; case __LINE__: TRICE64( ID( 6959), "rd:%e (%%e)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE64( ID( 6618), "rd:%f (%%f)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-555555555.555556 (%f)
        break; case __LINE__: TRICE64( ID( 6836), "rd:%g (%%g)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-5.555555555555556e+08 (%g)

        break; case __LINE__: TRICE32( ID( 3554), "msg:%u (%%u)\n", -1 ); //exp: time:32323232msg:4294967295 (%u)
        break; case __LINE__: TRICE32( ID( 2392), "msg:%b (%%b)\n", -1 ); //exp: time:32323232msg:11111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE32( ID( 7285), "msg:%o (%%o)\n", -1 ); //exp: time:32323232msg:37777777777 (%o)
        break; case __LINE__: TRICE32( ID( 1495), "msg:%O (%%O)\n", -1 ); //exp: time:32323232msg:0o37777777777 (%O)
        break; case __LINE__: TRICE32( ID( 4786), "msg:%X (%%X)\n", -1 ); //exp: time:32323232msg:FFFFFFFF (%X)
        break; case __LINE__: TRICE32( ID( 1424), "msg:%x (%%x)\n", -1 ); //exp: time:32323232msg:ffffffff (%x)
        break; case __LINE__: TRICE32( ID( 3236), "msg:%d (%%d)\n", -1 ); //exp: time:32323232msg:-1 (%d)
        break; case __LINE__: TRICE64( ID( 3626), "msg:%u (%%u)\n", -1 ); //exp: time:32323232msg:18446744073709551615 (%u)
        break; case __LINE__: TRICE64( ID( 4709), "msg:%b (%%b)\n", -1 ); //exp: time:32323232msg:1111111111111111111111111111111111111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE64( ID( 2601), "msg:%o (%%o)\n", -1 ); //exp: time:32323232msg:1777777777777777777777 (%o)
        break; case __LINE__: TRICE64( ID( 7087), "msg:%O (%%O)\n", -1 ); //exp: time:32323232msg:0o1777777777777777777777 (%O)
        break; case __LINE__: TRICE64( ID( 4742), "msg:%X (%%X)\n", -1 ); //exp: time:32323232msg:FFFFFFFFFFFFFFFF (%X)
        break; case __LINE__: TRICE64( ID( 6030), "msg:%x (%%x)\n", -1 ); //exp: time:32323232msg:ffffffffffffffff (%x)

//      break; case __LINE__: 
//  #ifdef XTEA_ENCRYPT_KEY
//          TRICE0 (Id( 4367), "--------------------------------------------------\n" );
//          {
//              uint32_t by[8] = {1,2,3,4,5,6};
//              int count = (sizeof(by) + 7) & ~7; // only multiple of 8 encryptable
//              TRICE32(  Id( 4122), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              TRICE32( Id( 1683), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
//              XTEAEncrypt(by, count>>2);
//              TRICE32( Id( 5597), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
//              TRICE32(  Id( 1756), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              #ifdef XTEA_DECRYPT
//              TRICE16_1( Id( 6899), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
//              XTEADecrypt(by, count>>2);
//              TRICE16_1( Id( 1772), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
//              #endif
//              TRICE32(  Id( 4093), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//          }
//          TRICE0 (Id( 2388), "--------------------------------------------------\n" );
//  #endif
        break; case __LINE__: showTriceDepth();

        break; case __LINE__: TRICE( ID( 1093), "sig:Some time measurements\n" );
        break; case __LINE__: TRICE( ID( 4644), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 1222), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 1993), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 4930), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE64( ID( 3976), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 2844), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( ID( 5548), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID( 2333), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( ID( 6847), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID( 3181), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE64( ID( 1014), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 4218), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID( 3250), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 4900), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( ID( 7517), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 6657), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 3934), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( ID( 6374), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 1155), "rd:TRICE32_1 line %d\n", __LINE__ );
        break; case __LINE__: TRICE64_1( ID( 6237), "rd:TRICE64_1 %d\n", __LINE__ );
        break; case __LINE__: TRICE32_2( ID( 2532), "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
        break; case __LINE__: TRICE64_2( ID( 3510), "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );


        break; case __LINE__: TRICE( ID( 3612), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
                                  s = "AAAAAAAAAAAA";
                                  TRICE32( ID( 3965), "dbg:len=%u:", strlen(s) );
                                  TRICE_S( ID( 7430), "sig:%s\n", s );
                              }
        break; case __LINE__: {
                s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
                          }
        break; case __LINE__: TRICE32( ID( 7639), "dbg:len=%u:", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 2697), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID( 7861), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
                          }
        break; case __LINE__: TRICE32( ID( 4202), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 7919), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID( 1693), "sig:Runtime generated strings\n" );
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
        break; case __LINE__: TRICE32( ID( 5914), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 7178), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID( 4609), "sig:Runtime generated strings\n" );
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
        break; case __LINE__: TRICE32( ID( 5590), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 3523), "sig:%s\n", s );

        break; case __LINE__: TRICE( ID( 7846), "sig:Integer (indent, base, sign) - see https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/\n" );
        break; case __LINE__: TRICE( ID( 4581), "rd: 15 	%d 	Base 10\n", 15 );                                        //ex_: time:32323232rd: 15 	15 	Base 10                                   
        break; case __LINE__: TRICE( ID( 1487), "rd: +15 	%+d 	Always show sign\n", 15 );                           //ex_: time:32323232rd: +15 	+15 	Always show sign                          
        break; case __LINE__: TRICE( ID( 6540), "rd:   15 	%4d 	Pad with spaces (width 4, right justified)\n", 15 ); //ex_: time:32323232rd:   15	  15 	Pad with spaces (width 4, right justified)
        break; case __LINE__: TRICE( ID( 1906), "rd: 15   	%-4d 	Pad with spaces (width 4, left justified)\n", 15 );  //ex_: time:32323232rd: 15 	15   	Pad with spaces (width 4, left justified) 
        break; case __LINE__: TRICE( ID( 6711), "rd: 0015 	%04d 	Pad with zeroes (width 4)\n", 15 );                  //ex_: time:32323232rd: 0015 	0015 	Pad with zeroes (width 4)                 
        break; case __LINE__: TRICE( ID( 4464), "rd: 1111 	%b 	Base 2\n", 15 );                                         //ex_: time:32323232rd: 1111 	1111    Base 2                                    
        break; case __LINE__: TRICE( ID( 5633), "rd: 17 	%o 	Base 8\n", 15 );                                         //ex_: time:32323232rd: 17 	17      Base 8                                    
        break; case __LINE__: TRICE( ID( 2407), "rd: f 	%x 	Base 16, lowercase\n", 15 );                             //ex_: time:32323232rd: f 	f 	Base 16, lowercase                        
        break; case __LINE__: TRICE( ID( 2686), "rd: F 	%X 	Base 16, uppercase\n", 15 );                             //ex_: time:32323232rd: F 	F 	Base 16, uppercase                        
        break; case __LINE__: TRICE( ID( 4373), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", 15 );                   //ex_: time:32323232rd: 0xf 	0xf 	Base 16, with leading 0x                  

        break; case __LINE__: TRICE( ID( 2597), "sig:Character (quoted, Unicode)\n" );
        break; case __LINE__: TRICE( ID( 5284), "rd: A 	%c 	Character\n", 'A' );                                     //exp: time:32323232rd: A 	A 	Character
        break; case __LINE__: TRICE( ID( 4111), "rd: 'A' 	%q 	Quoted character\n", 'A' );                              //exp: time:32323232rd: 'A' 	'A' 	Quoted character
        break; case __LINE__: TRICE( ID( 6424), "rd: U+0041 	%U 	Unicode\n", 'A' );                        // todo
        break; case __LINE__: TRICE( ID( 7276), "rd: U+0041 'A' 	%#U 	Unicode with character\n", 'A' ); // todo

        break; case __LINE__: TRICE( ID( 5878), "sig:Boolean (true/false)\n" );
        break; case __LINE__: TRICE( ID( 3448), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 ); //exp: time:32323232rd:Use %t to format a boolean as true (true) or false (false).

        break; case __LINE__: TRICE( ID( 1490), "sig:Pointer (hex)\n" );
        break; case __LINE__: TRICE8 ( ID( 3844), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );               //exp: time:32323232rd:Use %p to format a pointer in base 16 notation with leading 0x. (84)              
        break; case __LINE__: TRICE16( ID( 3971), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );             //exp: time:32323232rd:Use %p to format a pointer in base 16 notation with leading 0x. (1234)            
        break; case __LINE__: TRICE32( ID( 2834), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );         //exp: time:32323232rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        
        break; case __LINE__: TRICE64( ID( 4120), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 ); //exp: time:32323232rd:Use %p to format a pointer in base 16 notation with leading 0x. (1122334455667788)
        break; case __LINE__: TRICE  ( ID( 7050), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);          //exp: time:32323232rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        

        break; case __LINE__: TRICE( ID( 4136), "sig:Float (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE( ID( 7833), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(123.456) );                       //exp: time:32323232rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE( ID( 2188), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(123.456) );                //exp: time:32323232rd: 123.456000		123.456001 	%f Decimal point, no exponent
        break; case __LINE__: TRICE( ID( 7314), "rd: 123.46			%.2f		%%.2f Default width, precision 2\n", aFloat(123.456) );      //exp: time:32323232rd: 123.46			123.46		%.2f Default width, precision 2
        break; case __LINE__: TRICE( ID( 7996), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(123.456) );                    //exp: time:32323232rd: ␣␣123.46		  123.46	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE( ID( 2806), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(123.456) ); //exp: time:32323232rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE( ID( 1878), "sig:Double (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE64( ID( 7362), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(123.456) );                   //exp: time:32323232rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE64( ID( 2391), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(123.456) );                //exp: time:32323232rd: 123.456000		123.456000 	%f Decimal point, no exponent
        break; case __LINE__: TRICE64( ID( 6363), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(123.456) );      //exp: time:32323232rd: 123.46			123.46    	%.2f Default width, precision 2
        break; case __LINE__: TRICE64( ID( 7719), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(123.456) );                //exp: time:32323232rd: ␣␣123.46		  123.46 	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE64( ID( 3937), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(123.456) ); //exp: time:32323232rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE( ID( 1730), "sig:String or byte slice (quote, indent, hex)\n" );
        break; case __LINE__: s = "café"; TRICE_S( ID( 6536), "rd: café 			%s		Plain string\n", s );           //exp: time:32323232rd: café 			café		Plain string
        break; case __LINE__: s = "café"; TRICE_S( ID( 2727), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );  //exp: time:32323232rd: ␣␣café 		  café 		Width 6, right justify
        break; case __LINE__: s = "café"; TRICE_S( ID( 6807), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );   //exp: time:32323232rd: café␣␣ 		café   		Width 6, left justify
        break; case __LINE__: s = "café"; TRICE_S( ID( 6876), "rd: \"café\" 		%q		Quoted string\n", s );          //exp: time:32323232rd: \"café\" 		"café"		Quoted string
        break; case __LINE__: s = "café"; TRICE_S( ID( 3564), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );    //exp: time:32323232rd: 636166c3a9 		636166c3a9	Hex dump of byte values 
        break; case __LINE__: s = "café"; TRICE_S( ID( 3796), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );       //exp: time:32323232rd: 63 61 66 c3 a9 	63 61 66 c3 a9	Hex dump with spaces

#ifdef TRICE_CGO
        break; case __LINE__: TRICE8   ( id( 3601), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE8_1 ( id( 6915), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE8   ( Id( 5582), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE8_1 ( Id( 2133), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE8   ( ID( 5449), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE8_1 ( ID( 1888), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice8   ( iD( 3429), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice8_1 ( iD( 1217), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice8   ( iD( 7903), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice8_1 ( iD( 6888), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice8   ( iD( 2443), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice8_1 ( iD( 4184), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200

        break; case __LINE__: TRICE16  ( id( 3600), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE16_1( id( 3946), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE16  ( Id( 5636), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE16_1( Id( 5193), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE16  ( ID( 4562), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE16_1( ID( 2520), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice16  ( iD( 4216), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice16_1( iD( 2106), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice16  ( iD( 6995), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice16_1( iD( 3727), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice16  ( iD( 2592), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice16_1( iD( 2990), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200

        break; case __LINE__: TRICE32  ( id( 4148), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE32_1( id( 4441), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE32  ( Id( 4636), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE32_1( Id( 6661), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE32  ( ID( 1965), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE32_1( ID( 1720), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice32  ( iD( 3675), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice32_1( iD( 6602), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice32  ( iD( 7072), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice32_1( iD( 5293), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice32  ( iD( 2116), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice32_1( iD( 3888), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200

        break; case __LINE__: TRICE64  ( id( 3044), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE64_1( id( 3324), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE64  ( Id( 2233), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE64_1( Id( 7906), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE64  ( ID( 5102), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE64_1( ID( 4651), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice64  ( iD( 4851), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice64_1( iD( 2595), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice64  ( iD( 3412), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice64_1( iD( 4427), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice64  ( iD( 5784), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice64_1( iD( 2855), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200

        // mixed 16-bit and 32-bit timestamp 
        break; case __LINE__: TRice8( iD( 3249), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice8( iD( 3895), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice8( iD( 2991), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice8( iD( 5032), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8( iD( 2573), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8( iD( 1681), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8( iD( 7881), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8( iD( 6796), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8( iD( 6294), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8( iD( 7826), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8( iD( 5871), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8( iD( 3761), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16( iD( 4103), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice16( iD( 5577), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice16( iD( 2101), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice16( iD( 6496), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16( iD( 1793), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16( iD( 7053), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16( iD( 2374), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16( iD( 3830), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16( iD( 4125), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16( iD( 1512), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16( iD( 7708), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16( iD( 2480), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32( iD( 2998), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice32( iD( 4674), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice32( iD( 5228), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice32( iD( 1277), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32( iD( 7766), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32( iD( 2431), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32( iD( 3380), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32( iD( 3596), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32( iD( 3607), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32( iD( 4083), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32( iD( 3813), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32( iD( 3039), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64( iD( 4446), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice64( iD( 7648), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice64( iD( 4841), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice64( iD( 7821), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64( iD( 3443), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64( iD( 1159), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64( iD( 7292), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64( iD( 4568), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64( iD( 2163), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64( iD( 6517), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64( iD( 6916), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64( iD( 4163), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice8_1 ( iD( 5939), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice8_2 ( iD( 3907), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice8_3 ( iD( 7116), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice8_4 ( iD( 1319), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8_5 ( iD( 3994), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8_6 ( iD( 2266), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8_7 ( iD( 3728), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8_8 ( iD( 2412), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8_9 ( iD( 7813), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8_10( iD( 4172), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8_11( iD( 2763), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8_12( iD( 3296), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16_1 ( iD( 7029), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice16_2 ( iD( 6007), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice16_3 ( iD( 4174), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice16_4 ( iD( 4333), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16_5 ( iD( 1048), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16_6 ( iD( 3439), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16_7 ( iD( 2250), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16_8 ( iD( 7591), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16_9 ( iD( 1085), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16_10( iD( 3598), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16_11( iD( 7935), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16_12( iD( 7033), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32_1 ( iD( 6961), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice32_2 ( iD( 6531), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice32_3 ( iD( 5933), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice32_4 ( iD( 1432), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32_5 ( iD( 7690), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32_6 ( iD( 7126), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32_7 ( iD( 4377), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32_8 ( iD( 4690), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32_9 ( iD( 1158), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32_10( iD( 5008), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32_11( iD( 2093), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32_12( iD( 1027), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64_1 ( iD( 2585), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice64_2 ( iD( 3145), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice64_3 ( iD( 7995), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice64_4 ( iD( 1120), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64_5 ( iD( 6651), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64_6 ( iD( 6411), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64_7 ( iD( 6912), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64_8 ( iD( 6815), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64_9 ( iD( 7877), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64_10( iD( 2536), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64_11( iD( 2415), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64_12( iD( 4285), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        // use cases for 8-bit data
        break; case __LINE__: trice8     ( iD( 6434), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice8     ( iD( 4189), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice8     ( iD( 1770), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice8_1   ( iD( 6212), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice8_1   ( iD( 2129), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice8_1   ( iD( 3008), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 16-bit data
        break; case __LINE__: trice16    ( iD( 4395), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice16    ( iD( 7852), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice16    ( iD( 5189), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice16_1  ( iD( 3955), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice16_1  ( iD( 5047), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice16_1  ( iD( 6873), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 32-bit data
        break; case __LINE__: trice32    ( iD( 7468), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice32    ( iD( 6088), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice32    ( iD( 7870), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice32_1  ( iD( 3446), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice32_1  ( iD( 2903), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice32_1  ( iD( 7061), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 64-bit data
        break; case __LINE__: trice64    ( iD( 7653), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice64    ( iD( 3132), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice64    ( iD( 4371), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice64_1  ( iD( 6487), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice64_1  ( iD( 1874), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice64_1  ( iD( 7793), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // legacy use cases
        break; case __LINE__: TRICE      ( ID( 5003),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE      ( id( 4606), "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: TRICE      ( Id( 7369), "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: TRICE      ( ID( 5147), "value=%d\n", -2 ); //exp: time:32323232value=-2

        // default stamp
        break; case __LINE__: TRICE8     ( ID( 7179),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE8_1   ( ID( 7604),        "value=%d\n", -2 ); //exp: time:32323232value=-2

        // no stamp
        break; case __LINE__: trice8     ( iD( 3671),        "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: trice8_1   ( iD( 2424),        "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: TRICE8     ( id( 5819), "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: TRICE8_1   ( id( 4772), "value=%d\n", -2 ); //exp: time:        value=-2

        // 16-bit stamp:
        break; case __LINE__: Trice8     ( iD( 2796),        "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: Trice8_1   ( iD( 4410),        "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: TRICE8     ( Id( 6744), "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: TRICE8_1   ( Id( 5663), "value=%d\n", -2 ); //exp: time:    1616value=-2

        // 32-bit stamp:
        break; case __LINE__: TRice8     ( iD( 4557),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRice8_1   ( iD( 2045),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE8     ( ID( 6299), "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE8_1   ( ID( 1560), "value=%d\n", -2 ); //exp: time:32323232value=-2

        break; case __LINE__: TRICE( ID( 4846), "FATAL:magenta+b:red\n" );           // exp: time:32323232FATAL:magenta+b:red
        break; case __LINE__: TRICE( ID( 7641), "CRITICAL:red+i:default+h\n" );      // exp: time:32323232CRITICAL:red+i:default+h
        break; case __LINE__: TRICE( ID( 4481), "EMERGENCY:red+i:blue\n" );          // exp: time:32323232EMERGENCY:red+i:blue
        break; case __LINE__: TRICE( ID( 2371), "ERROR:11:red\n" );                  // exp: time:32323232ERROR:11:red
        break; case __LINE__: TRICE( ID( 1053), "WARNING:11+i:red\n" );              // exp: time:32323232WARNING:11+i:red
        break; case __LINE__: TRICE( ID( 3506), "ATTENTION:11:green\n" );            // exp: time:32323232ATTENTION:11:green
        break; case __LINE__: TRICE( ID( 4435), "INFO:cyan+b:default+h\n" );         // exp: time:32323232INFO:cyan+b:default+h
        break; case __LINE__: TRICE( ID( 2245), "DEBUG:130+i\n" );                   // exp: time:32323232DEBUG:130+i
        break; case __LINE__: TRICE( ID( 3231), "TRACE:default+i:default+h\n" );     // exp: time:32323232TRACE:default+i:default+h
        break; case __LINE__: TRICE( ID( 6369), "TIME:blue+i:blue+h\n" );            // exp: time:32323232TIME:blue+i:blue+h
        break; case __LINE__: TRICE( ID( 3474), "MESSAGE:green+h:black\n" );         // exp: time:32323232MESSAGE:green+h:black
        break; case __LINE__: TRICE( ID( 4245), "READ:black+i:yellow+h\n" );         // exp: time:32323232READ:black+i:yellow+h
        break; case __LINE__: TRICE( ID( 6433), "WRITE:black+u:yellow+h\n" );        // exp: time:32323232WRITE:black+u:yellow+h
        break; case __LINE__: TRICE( ID( 4794), "RECEIVE:black+h:black\n" );         // exp: time:32323232RECEIVE:black+h:black
        break; case __LINE__: TRICE( ID( 1698), "TRANSMIT:black:black+h\n" );        // exp: time:32323232TRANSMIT:black:black+h
        break; case __LINE__: TRICE( ID( 5568), "DIAG:yellow+i:default+h\n" );       // exp: time:32323232DIAG:yellow+i:default+h
        break; case __LINE__: TRICE( ID( 2311), "INTERRUPT:magenta+i:default+h\n" ); // exp: time:32323232INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE( ID( 2881), "SIGNAL:118+i\n" );                  // exp: time:32323232SIGNAL:118+i
        break; case __LINE__: TRICE( ID( 7789), "TEST:yellow+h:black\n" );           // exp: time:32323232TEST:yellow+h:black
        break; case __LINE__: TRICE( ID( 5269), "DEFAULT:off\n" );                   // exp: time:32323232DEFAULT:off
        break; case __LINE__: TRICE( ID( 3289), "NOTICE:blue:white+h\n" );           // exp: time:32323232NOTICE:blue:white+h
        break; case __LINE__: TRICE( ID( 5600), "ALERT:magenta:magenta+h\n" );       // exp: time:32323232ALERT:magenta:magenta+h
        break; case __LINE__: TRICE( ID( 6850), "ASSERT:yellow+i:blue\n" );          // exp: time:32323232ASSERT:yellow+i:blue
        break; case __LINE__: TRICE( ID( 7849), "ALARM:red+i:white+h\n" );           // exp: time:32323232ALARM:red+i:white+h
        break; case __LINE__: TRICE( ID( 4887), "CYCLE:blue+i:default+h\n" );        // exp: time:32323232CYCLE:blue+i:default+h
        break; case __LINE__: TRICE( ID( 3186), "VERBOSE:blue:default\n" );          // exp: time:32323232VERBOSE:blue:default
        break; case __LINE__: TRICE( ID( 5347), "fatal:magenta+b:red\n" );           // exp: time:32323232fatal:magenta+b:red
        break; case __LINE__: TRICE( ID( 6712), "critical:red+i:default+h\n" );      // exp: time:32323232critical:red+i:default+h
        break; case __LINE__: TRICE( ID( 1413), "emergency:red+i:blue\n" );          // exp: time:32323232emergency:red+i:blue
        break; case __LINE__: TRICE( ID( 1059), "error:11:red\n" );                  // exp: time:32323232error:11:red
        break; case __LINE__: TRICE( ID( 6208), "warning:11+i:red\n" );              // exp: time:32323232warning:11+i:red
        break; case __LINE__: TRICE( ID( 4800), "attention:11:green\n" );            // exp: time:32323232attention:11:green
        break; case __LINE__: TRICE( ID( 4605), "info:cyan+b:default+h\n" );         // exp: time:32323232info:cyan+b:default+h
        break; case __LINE__: TRICE( ID( 3339), "debug:130+i\n" );                   // exp: time:32323232debug:130+i
        break; case __LINE__: TRICE( ID( 2565), "trace:default+i:default+h\n" );     // exp: time:32323232trace:default+i:default+h
        break; case __LINE__: TRICE( ID( 5119), "time:blue+i:blue+h\n" );            // exp: time:32323232time:blue+i:blue+h
        break; case __LINE__: TRICE( ID( 4749), "message:green+h:black\n" );         // exp: time:32323232message:green+h:black
        break; case __LINE__: TRICE( ID( 5888), "read:black+i:yellow+h\n" );         // exp: time:32323232read:black+i:yellow+h
        break; case __LINE__: TRICE( ID( 4948), "write:black+u:yellow+h\n" );        // exp: time:32323232write:black+u:yellow+h
        break; case __LINE__: TRICE( ID( 5940), "receive:black+h:black\n" );         // exp: time:32323232receive:black+h:black
        break; case __LINE__: TRICE( ID( 4554), "transmit:black:black+h\n" );        // exp: time:32323232transmit:black:black+h
        break; case __LINE__: TRICE( ID( 4280), "diag:yellow+i:default+h\n" );       // exp: time:32323232diag:yellow+i:default+h
        break; case __LINE__: TRICE( ID( 2343), "interrupt:magenta+i:default+h\n" ); // exp: time:32323232interrupt:magenta+i:default+h
        break; case __LINE__: TRICE( ID( 5200), "signal:118+i\n" );                  // exp: time:32323232signal:118+i
        break; case __LINE__: TRICE( ID( 2489), "test:yellow+h:black\n" );           // exp: time:32323232test:yellow+h:black
        break; case __LINE__: TRICE( ID( 2115), "default:off\n" );                   // exp: time:32323232default:off
        break; case __LINE__: TRICE( ID( 4879), "notice:blue:white+h\n" );           // exp: time:32323232notice:blue:white+h
        break; case __LINE__: TRICE( ID( 3512), "alert:magenta:magenta+h\n" );       // exp: time:32323232alert:magenta:magenta+h
        break; case __LINE__: TRICE( ID( 3484), "assert:yellow+i:blue\n" );          // exp: time:32323232assert:yellow+i:blue
        break; case __LINE__: TRICE( ID( 4712), "alarm:red+i:white+h\n" );           // exp: time:32323232alarm:red+i:white+h
        break; case __LINE__: TRICE( ID( 3771), "cycle:blue+i:default+h\n" );        // exp: time:32323232cycle:blue+i:default+h
        break; case __LINE__: TRICE( ID( 7681), "verbose:blue:default\n" );          // exp: time:32323232verbose:blue:default
        break; case __LINE__: TRICE64( id( 3530), "msg:%d (%%d)\n", -1 );     //exp: time:        msg:-1 (%d)

        break; case __LINE__: TRICE( ID( 5717), "sig:TRICE with 1 to 12 values\n" );
        break; case __LINE__: TRICE( ID( 6203), "rd:TRICE %d\n", -1 );
        break; case __LINE__: TRICE( ID( 5744), "rd:TRICE %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE( ID( 6457), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE( ID( 2229), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE( ID( 6246), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE( ID( 7723), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE( ID( 4343), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE( ID( 6593), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE( ID( 4450), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE( ID( 7239), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE( ID( 2331), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE( ID( 3337), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE( ID( 5771),  "sig:TRICE_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE_1( ID( 7213), "rd:TRICE_1 %d\n", 1 );
        break; case __LINE__: TRICE_2( ID( 1639), "rd:TRICE_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE_3( ID( 4024), "rd:TRICE_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE_4( ID( 5500), "rd:TRICE_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE_5( ID( 2252), "rd:TRICE_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE_6( ID( 2897), "rd:TRICE_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE_7( ID( 4907), "rd:TRICE_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE_8( ID( 1186), "rd:TRICE_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE_9( ID( 4597), "rd:TRICE_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE_10( ID( 2845), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE_11( ID( 3193), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE_12( ID( 4451), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );				

        break; case __LINE__: TRICE( ID( 7099), "sig:TRICE8 with 1 to 12 values\n" );
        break; case __LINE__: TRICE8( ID( 4400), "rd:TRICE8 %d\n", -1 );
        break; case __LINE__: TRICE8( ID( 3738), "rd:TRICE8 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE8( ID( 6167), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8( ID( 6828), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8( ID( 2196), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8( ID( 1047), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8( ID( 4328), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE8( ID( 2579), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8( ID( 5549), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8( ID( 2766), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE8( ID( 5123), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8( ID( 4058), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE( ID( 3867), "sig:TRICE8_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE8_1 ( ID( 1031), "rd:TRICE8_1 %d\n", 1 );
        break; case __LINE__: TRICE8_2 ( ID( 7855), "rd:TRICE8_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE8_3 ( ID( 2548), "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE8_4 ( ID( 5952), "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE8_5 ( ID( 1173), "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE8_6 ( ID( 7309), "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE8_7 ( ID( 6588), "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8 ( ID( 7482), "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE8_9 ( ID( 7423), "rd:TRICE8_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE8_10( ID( 4046), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE8_11( ID( 6367), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE8_12( ID( 5131), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE( ID( 5101), "sig:TRICE16 with 1 to 12 values\n" );
        break; case __LINE__: TRICE16( ID( 5297), "rd:TRICE16 %d\n", -1 );
        break; case __LINE__: TRICE16( ID( 4620), "rd:TRICE16 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 2745), "rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 3239), "rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 5191), "rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 2022), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 6484), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID( 3182), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 1077), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 3837), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID( 3462), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 3828), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID( 3170), "sig:TRICE16_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE16_1 ( ID( 6021), "rd:TRICE16_1 %d\n", 1 );
        break; case __LINE__: TRICE16_2 ( ID( 4633), "rd:TRICE16_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE16_3 ( ID( 1185), "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE16_4 ( ID( 5218), "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE16_5 ( ID( 2920), "rd:TRICE16_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE16_6 ( ID( 5346), "rd:TRICE16_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE16_7 ( ID( 4274), "rd:TRICE16_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE16_8 ( ID( 3658), "rd:TRICE16_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE16_9 ( ID( 4714), "rd:TRICE16_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE16_10( ID( 2051), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE16_11( ID( 3520), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE16_12( ID( 1759), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE  ( ID( 3921), "sig:TRICE16 with 1 to 12 pointer\n" );
        break; case __LINE__: TRICE16( ID( 3154), "rd:TRICE16 %p\n", -1 );
        break; case __LINE__: TRICE16( ID( 4361), "rd:TRICE16 %p, %p\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 2278), "rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 2276), "rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 5756), "rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 1859), "rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 4939), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID( 7154), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 2147), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 1218), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID( 5165), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 2865), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  ( ID( 3227), "sig:TRICE16 with 1 to 12 hex\n" );
        break; case __LINE__: TRICE16( ID( 4509), "rd:TRICE16 %X\n", -1 );
        break; case __LINE__: TRICE16( ID( 3960), "rd:TRICE16 %X, %X\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 3042), "rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 5403), "rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 1369), "rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 6114), "rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 2485), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID( 7140), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 2723), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 2081), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID( 3272), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 3779), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  ( ID( 6102), "sig:TRICE32 with 1 to 12 values\n" );
        break; case __LINE__: TRICE32( ID( 3213), "rd:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32( ID( 5556), "rd:TRICE32 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE32( ID( 2777), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32( ID( 1284), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32( ID( 4338), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32( ID( 5781), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32( ID( 6024), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE32( ID( 3454), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32( ID( 7867), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32( ID( 1419), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE32( ID( 3156), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32( ID( 6756), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID( 2460), "signal:TRICE32_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE32_1 ( ID( 7301), "rd:TRICE32_1 %d\n", 1 );
        break; case __LINE__: TRICE32_2 ( ID( 7041), "rd:TRICE32_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE32_3 ( ID( 1399), "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE32_4 ( ID( 2063), "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE32_5 ( ID( 4890), "rd:TRICE32_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE32_6 ( ID( 7352), "rd:TRICE32_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE32_7 ( ID( 3552), "rd:TRICE32_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE32_8 ( ID( 6288), "rd:TRICE32_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE32_9 ( ID( 5747), "rd:TRICE32_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE32_10( ID( 4440), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE32_11( ID( 7534), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE32_12( ID( 5812), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE  ( ID( 1513), "sig:TRICE64 with 1 to 12 values\n" );
        break; case __LINE__: TRICE64( ID( 3635), "rd:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64( ID( 7122), "rd:TRICE64 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE64( ID( 4969), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64( ID( 1123), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64( ID( 5432), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64( ID( 6729), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64( ID( 7564), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE64( ID( 1281), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64( ID( 5950), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64( ID( 1537), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE64( ID( 4685), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64( ID( 2105), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID( 4622), "sig:TRICE64_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE64_1 ( ID( 5634), "rd:TRICE64_1 %d\n", 1 );
        break; case __LINE__: TRICE64_2 ( ID( 2517), "rd:TRICE64_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE64_2 ( ID( 7806), "rd:TRICE64_2 %x, %x\n", 0x1122334455667788, 0x0102030405060708 );
        break; case __LINE__: TRICE64_3 ( ID( 7798), "rd:TRICE64_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE64_4 ( ID( 5430), "rd:TRICE64_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE64_5 ( ID( 7470), "rd:TRICE64_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE64_6 ( ID( 2811), "rd:TRICE64_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE64_7 ( ID( 4101), "rd:TRICE64_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE64_8 ( ID( 3620), "rd:TRICE64_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE64_9 ( ID( 5177), "rd:TRICE64_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE64_10( ID( 2717), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE64_11( ID( 6242), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE64_12( ID( 4561), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE( ID( 1359), "sig:Colors and ticks\n" );
        break; case __LINE__: TRICE( ID( 5557), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 3545), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 6343), "dbg:12345 as 16bit is %016b\n", 12345);
        break; case __LINE__: TRICE( ID( 3480), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 3699), "sig:This ASSERT error is just a demo and no real error:\n");
        break; case __LINE__: TRICE( ID( 5136), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 7532), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 5444), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 2731), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 4023), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 7306), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 4086), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 5865), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 6840), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 2563), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 5955), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 7489), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 2862), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 7592), "time:TRICE32_1  message, SysTick is %6u\n", SYSTICKVAL);

        break; case __LINE__: TRICE( ID( 1295), "sig:Various ranges and formats\n" );
        break; case __LINE__: TRICE8 ( ID( 1197), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 5261), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 7651), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 7961), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 7580), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 1593), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 7978), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 4621), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 2577), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE16( ID( 5050), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16( ID( 5544), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16( ID( 4068), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE32( ID( 6126), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32( ID( 2148), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32( ID( 5118), "att:TRICE32 %40b\n", 0xAAAAAAAA);
        break; case __LINE__: TRICE64( ID( 2890), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64( ID( 5226), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64( ID( 1840), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
        break; case __LINE__: TRICE( ID( 1703), "sig:Legacy TRICE8\n" );
        break; case __LINE__: TRICE8_1( ID( 2330), "tst:TRICE8_1 %02x\n", 0xA1);
        break; case __LINE__: TRICE8_2( ID( 4273), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
        break; case __LINE__: TRICE8_3( ID( 4661), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
        break; case __LINE__: TRICE8_4( ID( 6062), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
        break; case __LINE__: TRICE8_5( ID( 4496), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
        break; case __LINE__: TRICE8_6( ID( 3164), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
        break; case __LINE__: TRICE8_7( ID( 4918), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
        break; case __LINE__: TRICE8_8( ID( 3957), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
        break; case __LINE__: TRICE8_1( ID( 7016), "tst:TRICE8_1 %d\n",                      -1);
        break; case __LINE__: TRICE8_2( ID( 2369), "tst:TRICE8_2 %d %d\n",                   -1, -2);
        break; case __LINE__: TRICE8_3( ID( 5416), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
        break; case __LINE__: TRICE8_4( ID( 3437), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
        break; case __LINE__: TRICE8_5( ID( 1669), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
        break; case __LINE__: TRICE8_6( ID( 6735), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
        break; case __LINE__: TRICE8_7( ID( 3974), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE8_8( ID( 3910), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
        break; case __LINE__: TRICE8_1( ID( 5096), "tst:TRICE8_1 %d\n", 1);
        break; case __LINE__: TRICE8_2( ID( 2767), "tst:TRICE8_2 %d %d\n", 1, 2);
        break; case __LINE__: TRICE8_3( ID( 1454), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
        break; case __LINE__: TRICE8_4( ID( 1429), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
        break; case __LINE__: TRICE8_5( ID( 4840), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
        break; case __LINE__: TRICE8_6( ID( 1287), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
        break; case __LINE__: TRICE8_7( ID( 3292), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8( ID( 7429), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
        break; case __LINE__: TRICE8_1( ID( 7528), "tst:TRICE8_1 %u\n", 201);
        break; case __LINE__: TRICE8_2( ID( 2347), "tst:TRICE8_2 %u %u\n", 201, 202);
        break; case __LINE__: TRICE8_3( ID( 1902), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
        break; case __LINE__: TRICE8_4( ID( 1357), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
        break; case __LINE__: TRICE8_5( ID( 3958), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
        break; case __LINE__: TRICE8_6( ID( 3102), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
        break; case __LINE__: TRICE8_7( ID( 1073), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
        break; case __LINE__: TRICE8_8( ID( 2290), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
        break; case __LINE__: TRICE8_8( ID( 3017), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);

        break; case __LINE__: TRICE    ( ID( 1718), "sig:Legacy TRICE16-64\n" );
        break; case __LINE__: TRICE16_1( ID( 6917), "tst:TRICE16_1 %d\n", -111);
        break; case __LINE__: TRICE16_2( ID( 7889), "tst:TRICE16_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_3( ID( 1237), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE16_4( ID( 2200), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_1( ID( 1163), "tst:TRICE32_1 %08x\n", 0x0123cafe);
        break; case __LINE__: TRICE32_1( ID( 4558), "tst:TRICE32_1 %d\n", -111);
        break; case __LINE__: TRICE32_2( ID( 3391), "tst:TRICE32_2 %x %x\n", -111, -222);
        break; case __LINE__: TRICE32_2( ID( 6835), "tst:TRICE32_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE32_3( ID( 1087), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
        break; case __LINE__: TRICE32_3( ID( 2932), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE32_4( ID( 3048), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_4( ID( 6276), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE64_1( ID( 6892), "tst:TRICE64_1 %d\n", -111);
        break; case __LINE__: TRICE64_2( ID( 7115), "tst:TRICE64_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_1( ID( 3491), "tst:TRICE16_1 %u\n", 60001);
        break; case __LINE__: TRICE16_2( ID( 7194), "tst:TRICE16_2 %u %u\n", 60001, 60002);
        break; case __LINE__: TRICE16_3( ID( 6313), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
        break; case __LINE__: TRICE16_4( ID( 7211), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
        break; case __LINE__: TRICE32_1( ID( 4423), "tst:TRICE32_1 %u\n", 4000000001);
        break; case __LINE__: TRICE32_2( ID( 5913), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
        break; case __LINE__: TRICE32_3( ID( 6192), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
        break; case __LINE__: TRICE32_4( ID( 6886), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
        break; case __LINE__: TRICE64_1( ID( 7693), "tst:TRICE64_1 %x\n", -1);
        break; case __LINE__: TRICE64_2( ID( 2709), "tst:TRICE64_2 %x %x\n", -1, -2);
        break; case __LINE__: TRICE64_1( ID( 7354), "tst:TRICE64_1 %u\n", -1);
        break; case __LINE__: TRICE64_2( ID( 3691), "tst:TRICE64_2 %u %u\n", -1, -2);

        break; case __LINE__: TRICE( ID( 2523), "sig:colored single letters and several TRICE macros in one line\n" );
        break; case __LINE__: TRICE( ID( 6063), "e:A");
        break; case __LINE__: TRICE( ID( 2569), "w:B");
        break; case __LINE__: TRICE( ID( 3646), "a:c");
        break; case __LINE__: TRICE( ID( 4788), "wr:d");
        break; case __LINE__: TRICE( ID( 3664), "rd:e\n");
        break; case __LINE__: TRICE( ID( 6537), "diag:f");
        break; case __LINE__: TRICE( ID( 7953), "d:G");
        break; case __LINE__: TRICE( ID( 4579), "t:H");
        break; case __LINE__: TRICE( ID( 2210), "time:i");
        break; case __LINE__: TRICE( ID( 3627), "message:J");
        break; case __LINE__: TRICE( ID( 2400), "dbg:k\n");
        break; case __LINE__: TRICE( ID( 6121), "1");
        break; case __LINE__: TRICE( ID( 5907), "2");
        break; case __LINE__: TRICE( ID( 7842), "3");
        break; case __LINE__: TRICE( ID( 3912), "4");
        break; case __LINE__: TRICE( ID( 4752), "e:7");
        break; case __LINE__: TRICE( ID( 1083), "m:12");
        break; case __LINE__: TRICE( ID( 2077), "m:123\n");
        break; case __LINE__: TRICE( ID( 7064), "e:A"); TRICE( ID( 7366), "w:B"); TRICE( ID( 7521), "a:c");
        break; case __LINE__: TRICE( ID( 3158), "wr:d"); TRICE( ID( 5877), "rd:e\n"); TRICE( ID( 1348), "diag:f");

        break; case __LINE__: TRICE( ID( 5398),  "sig:TRICE8 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE8( ID( 4984), "tst:TRICE8 %d\n", -1 );
        break; case __LINE__: TRICE8( ID( 2373), "tst:TRICE8 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8( ID( 4845), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8( ID( 2080), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8( ID( 6375), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8( ID( 2713), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8( ID( 5092), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8( ID( 3904), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8( ID( 7449), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8( ID( 1518), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8( ID( 6395), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8( ID( 3455), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8( ID( 4664), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_1 ( ID( 1990), "tst:TRICE8_1  %d\n", -1 );
        break; case __LINE__: TRICE8_2 ( ID( 2295), "tst:TRICE8_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8_3 ( ID( 6521), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8_4 ( ID( 1149), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8_5 ( ID( 5643), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8_6 ( ID( 1003), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8_7 ( ID( 1243), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8_8 ( ID( 7878), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8_9 ( ID( 4574), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8_10( ID( 2498), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8_11( ID( 6610), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8_12( ID( 3436), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8   ( ID( 3857), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_9 ( ID( 2605), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );

        break; case __LINE__: TRICE( ID( 4213), "sig:TRICE16 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE16( ID( 4650), "tst:TRICE16 %d\n", -1 );
        break; case __LINE__: TRICE16( ID( 3142), "tst:TRICE16 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 7519), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 5180), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 4155), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 5182), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 2417), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16( ID( 4564), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 1741), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 6514), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16( ID( 6441), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 5211), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16( ID( 2397), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        break; case __LINE__: TRICE16_1 ( ID( 2772), "tst:TRICE16_1  %d\n", -1 );
        break; case __LINE__: TRICE16_2 ( ID( 4555), "tst:TRICE16_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16_3 ( ID( 3874), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16_4 ( ID( 6421), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16_5 ( ID( 1944), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16_6 ( ID( 3411), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16_7 ( ID( 6726), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16_8 ( ID( 6644), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16_9 ( ID( 4289), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16_10( ID( 2508), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16_11( ID( 4409), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16_12( ID( 2616), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16_9 ( ID( 2895), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );

        break; case __LINE__: TRICE( ID( 3818), "sig:TRICE32 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE32( ID( 1111), "tst:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32( ID( 4854), "tst:TRICE32 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32( ID( 4773), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32( ID( 1072), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32( ID( 5480), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32( ID( 6258), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32( ID( 1373), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32( ID( 2476), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32( ID( 2462), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32( ID( 2564), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32( ID( 6084), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32( ID( 2065), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32( ID( 2582), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_1 ( ID( 6714), "tst:TRICE32_1  %d\n", -1 );
        break; case __LINE__: TRICE32_2 ( ID( 4767), "tst:TRICE32_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32_3 ( ID( 4178), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32_4 ( ID( 3628), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32_5 ( ID( 5453), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32_6 ( ID( 6887), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32_7 ( ID( 7763), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32_8 ( ID( 7426), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32_9 ( ID( 6342), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32_10( ID( 2911), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32_11( ID( 3743), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32_12( ID( 6393), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32   ( ID( 3859), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_12( ID( 6112), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));

        break; case __LINE__: TRICE( ID( 1505), "sig:TRICE64 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE64( ID( 5988), "tst:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64( ID( 3812), "tst:TRICE64 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64( ID( 5491), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64( ID( 7026), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64( ID( 6559), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64( ID( 7738), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64( ID( 5825), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64( ID( 6459), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64( ID( 6494), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64( ID( 5746), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64( ID( 7330), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64( ID( 2769), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64( ID( 4796), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break; case __LINE__: TRICE64_1 ( ID( 3390), "tst:TRICE64_1  %d\n", -1 );
        break; case __LINE__: TRICE64_2 ( ID( 7655), "tst:TRICE64_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64_3 ( ID( 6843), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64_4 ( ID( 3695), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64_5 ( ID( 2494), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64_6 ( ID( 2383), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64_7 ( ID( 1798), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64_8 ( ID( 1091), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64_9 ( ID( 7453), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64_10( ID( 1571), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64_11( ID( 1318), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64_12( ID( 7507), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64_12( ID( 6911), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));

        break; case __LINE__: TRICE( ID( 2468), "att:positive and negative float in format variants\n");
        break; case __LINE__: TRICE( ID( 6486), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 5682), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 3898), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 1500), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 1623), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 7158), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3967), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 5992), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 5487), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 2156), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 5520), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 7145), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 4785), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 2617), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 2527), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 4162), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3316), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 1226), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 6499), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 4381), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 5357), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 1480), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 2119), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 5429), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 3121), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 6709), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 6115), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 4192), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3879), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3379), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7394), "att:positive float & double in variants\n");
        break; case __LINE__: TRICE32_1( ID( 2118), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 7093), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 6918), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 4739), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 7765), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 6071), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 7402), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 6175), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 1939), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 3720), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 2481), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 2664), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 4438), "rd:TRICE float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 5990), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 5716), "rd:TRICE float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 7586), "rd:TRICE float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 5960), "rd:TRICE float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 3847), "rd:TRICE float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE64  ( ID( 4186), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 1988), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 3894), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 1609), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 4116), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 4025), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 6675), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 6748), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 4354), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 2935), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 6017), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 4208), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE( ID( 3797), "att:negative float & double\n" );
        break; case __LINE__: TRICE( ID( 2385), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7175), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 1264), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 6996), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7343), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7197), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
        break; case __LINE__: TRICE64( ID( 1121), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 1828), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 1414), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 6005), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 5910), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 6889), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );

        break; case __LINE__: TRICE  ( ID( 3094), "att:formatted float & double\n" );
        break; case __LINE__: TRICE32( ID( 2143), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 3444), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 1664), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 1006), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 6327), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 4893), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
        break; case __LINE__: TRICE64( ID( 7378), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 7240), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 3212), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 7974), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 3330), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 4895), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );

        break; case __LINE__: TRICE ( ID( 7722), "att:Various single arguments\n" );
        break; case __LINE__: TRICE8( ID( 4199), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE8( ID( 4535), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE8( ID( 3046), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 4266), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 3981), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 3890), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 2418), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 7776), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 6072), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 6720), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE16_1( ID( 6162), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE16_1( ID( 5971), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 1484), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 3984), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 3572), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 2323), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
        break; case __LINE__: TRICE( ID( 2288), "rd:TRICE line %t (%%t -1)\n", 0 );
        break; case __LINE__: TRICE( ID( 1603), "rd:TRICE line %t (%%t -1)\n", 2 );
        break; case __LINE__: TRICE( ID( 3926), "rd:TRICE line %u (%%u)\n", -1 );
        break; case __LINE__: TRICE( ID( 1009), "rd:TRICE line %b (%%b)\n", -2 );
        break; case __LINE__: TRICE( ID( 7914), "rd:TRICE line %o (%%o)\n", -3 );
        break; case __LINE__: TRICE( ID( 7537), "rd:TRICE line %x (%%x)\n", -4 );
        break; case __LINE__: TRICE( ID( 6516), "rd:TRICE line %d (%%d)\n", -5 );

        break; case __LINE__: TRICE_N( ID( 1779), "msg:%s\n", A, 0 );
        break; case __LINE__: TRICE_N( ID( 4061), "msg:%s\n", A, 1 );
        break; case __LINE__: TRICE_N( ID( 6117), "msg:%s\n", A, 2 );
        break; case __LINE__: TRICE_N( ID( 5075), "msg:%s\n", A, 3 );
        break; case __LINE__: TRICE_N( ID( 4433), "msg:%s\n", A, 4 );
        break; case __LINE__: TRICE_N( ID( 3385), "msg:%s\n", A, 5 );
        break; case __LINE__: TRICE_N( ID( 3247), "msg:%s\n", A, 6 );
        break; case __LINE__: TRICE_N( ID( 2259), "msg:%s\n", A, 7 );
        break; case __LINE__: TRICE_N( ID( 7780), "msg:%s\n", A, 8 );
        break; case __LINE__: TRICE_N( ID( 7824), "msg:%s\n", A, 9 );
        break; case __LINE__: TRICE_N( ID( 7643), "msg:%s\n", A, 10 );
        break; case __LINE__: TRICE_N( ID( 7451), "msg:%s\n", A, 11 );
        break; case __LINE__: TRICE_N( ID( 3690), "msg:%s\n", A, 12 );
        break; case __LINE__: TRICE_N( ID( 7218), "msg:%s\n", A, 13 );
        break; case __LINE__: TRICE_N( ID( 2724), "msg:%s\n", A, 14 );
        break; case __LINE__: TRICE_N( ID( 3702), "msg:%s\n", A, 15 );
        break; case __LINE__: TRICE_N( ID( 3373), "msg:%s\n", A, 16 );
        break; case __LINE__: TRICE_N( ID( 6677), "msg:%s\n", A, 17 );
        break; case __LINE__: TRICE_N( ID( 6913), "msg:%s\n", A, 18 );
        break; case __LINE__: TRICE_N( ID( 1605), "msg:%s\n", A, 19 );
        break; case __LINE__: TRICE_N( ID( 7060), "msg:%s\n", A, 120 );
        break; case __LINE__: TRICE_N( ID( 4857), "msg:%s\n", A, 121 );
        break; case __LINE__: TRICE_N( ID( 2778), "msg:%s\n", A, 122 );
        break; case __LINE__: TRICE_N( ID( 3163), "msg:%s\n", A, 123 );
        break; case __LINE__: TRICE_N( ID( 5044), "msg:%s\n", A, 124 );
        break; case __LINE__: TRICE_N( ID( 7581), "msg:%s\n", A, 125 );
        break; case __LINE__: TRICE_N( ID( 1645), "msg:%s\n", A, 126 );
        break; case __LINE__: TRICE_N( ID( 5685), "msg:%s\n", A, 127 );
        break; case __LINE__: TRICE_N( ID( 3329), "msg:%s\n", A, 128 );
        break; case __LINE__: TRICE_N( ID( 4278), "msg:%s\n", A, 129 );

    #endif // #ifdef TRICE_CGO
    }
}
