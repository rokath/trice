/*
volatile uint8_t led = 0;

void SetLED( uint8_t state ){
    led = state;
}

#define TriceRpc0( id, pfmt, n )

void triceRpcList( void ){
     TriceRpc0( id(0), "cmd:LED %d", SetLed); // experimental send command
}
*/

#include "cobsr.h"

void COBSRCheck( void* pTestValue, int byteWidth ){
    cobsr_encode_result COBSEncResult = {0};
    cobsr_decode_result COBSDecResult = {0};
    uint8_t COBSEncBuf[12], COBSDecBuf[12];
    //static int lastMs = 0;
    //extern int milliSecond;
    ////while( milliSecond < lastMs + 10 );
    //lastMs = milliSecond;
    switch( byteWidth ){
        case 1: TRICE16( Id( 47229), "msg:%02x -> ", *(uint8_t*)pTestValue ); break;
        case 2: TRICE32( Id( 59542), "msg:%04x -> ", *(uint16_t*)pTestValue ); break;
        default: return;
    }
    COBSEncResult = cobsr_encode( 
    COBSEncBuf, //void * dst_buf_ptr, 
    sizeof( COBSEncBuf ), // size_t dst_buf_len,
    pTestValue, // const void * src_ptr, 
    byteWidth ); // size_t src_len)
    if( COBSR_ENCODE_OK != COBSEncResult.status ){
        TRICE16( Id( 55427), "err: i = %d, COBSEncResult.status = %d\n", 0, COBSEncResult.status );
    }else{
        switch( COBSEncResult.out_len ){
            case 1: TRICE16( Id( 37961), "sig:%02x\n", COBSEncBuf[0]); break;
            case 2: TRICE16( Id( 41310), "sig:%02x %02x\n", COBSEncBuf[0], COBSEncBuf[1]); break;
            case 3: TRICE16( Id( 32787), "sig:%02x %02x %02x\n", COBSEncBuf[0], COBSEncBuf[1], COBSEncBuf[2]); break;
            case 4: TRICE16( Id( 37461), "sig:%02x %02x %02x %02x\n", COBSEncBuf[0], COBSEncBuf[1], COBSEncBuf[2], COBSEncBuf[3]); break;
        }
    }
    
    COBSDecResult = cobsr_decode(
    COBSDecBuf, // void * dst_buf_ptr, 
    sizeof( COBSDecBuf ), // size_t dst_buf_len,
    COBSEncBuf, // const void * src_ptr,
    COBSEncResult.out_len); // size_t src_len);
    
    if( COBSR_DECODE_OK != COBSDecResult.status ){
        TRICE16( Id( 35623), "err: i = %d, COBSDecResult.status = %d\n", 0, COBSDecResult.status );
    }else{
        if( (byteWidth != COBSDecResult.out_len)  ){
            TRICE16( Id( 53032), "err: COBSDecResult.out_len = %d != byteWidth = %x\n", COBSDecResult.out_len, byteWidth);
            return;
        }
        switch( byteWidth ) {
            case 1: if( *(uint8_t*)pTestValue != COBSDecBuf[0] ){
                // err
            }
            return;
            case 2: if( *(uint16_t*)pTestValue != *(uint16_t*)COBSDecBuf ){
                // err
            }
            return;
        }
    }
}

//uint8_t oneByteArray[] = {
    // 0,      1,      2,      3,   0xFC,   0xFD,   0xFE,   0xFF 
    // 00 -> 01 01
    // 01 -> 02 01
    // 02 -> 02
    // 03 -> 03
    // fc -> fc
    // fd -> fd
    // fe -> fe
    // ff -> ff
//    };
uint16_t twoByteArray[] = { 
    // 0x0000, 0x0001, 0x0002, 0x0003, 0x00fc, 0x00fd, 0x00fe, 0x00ff,
    // 0000 -> 01 01 01
    // 0001 -> 02 01 01
    // 0002 -> 02 02 01
    // 0003 -> 02 03 01
    // 00fc -> 02 fc 01
    // 00fd -> 02 fd 01
    // 00fe -> 02 fe 01
    // 00ff -> 02 ff 01

    // 0x0100, 0x0101, 0x0102, 0x0103, 0x01fc, 0x01fd, 0x01fe, 0x01ff,
    // 0100 -> 01 02 01
    // 0101 -> 03 01 01
    // 0102 -> 03 02 01
    // 0103 -> 03 03 01
    // 01fc -> 03 fc 01
    // 01fd -> 03 fd 01
    // 01fe -> 03 fe 01
    // 01ff -> 03 ff 01
  
    //0x0200, 0x0201, 0x0202, 0x0203, 0x02fc, 0x02fd, 0x02fe, 0x02ff,
    // 0200 -> 01 02
    // 0201 -> 03 01 02
    // 0202 -> 03 02 02
    // 0203 -> 03 03 02
    // 02fc -> 03 fc 02
    // 02fd -> 03 fd 02
    // 02fe -> 03 fe 02
    // 02ff -> 03 ff 02
        
    // 0x0300, 0x0301, 0x0302, 0x0303, 0x03fc, 0x03fd, 0x03fe, 0x03ff,
    // 0300 -> 01 03
    // 0301 -> 03 01
    // 0302 -> 03 02
    // 0303 -> 03 03
    // 03fc -> 03 fc
    // 03fd -> 03 fd
    // 03fe -> 03 fe
    // 03ff -> 03 ff

    // 0xfc00, 0xfc01, 0xfc02, 0xfc03, 0xfcfc, 0xfcfd, 0xfcfe, 0xfcff,
    // fc00 -> 01 fc
    // fc01 -> fc 01
    // fc02 -> fc 02
    // fc03 -> fc 03
    // fcfc -> fc fc
    // fcfd -> fc fd
    // fcfe -> fc fe
    // fcff -> fc ff
    
    // 0xfd00, 0xfd01, 0xfd02, 0xfd03, 0xfdfc, 0xfdfd, 0xfdfe, 0xfdff,
    // fd00 -> 01 fd
    // fd01 -> fd 01
    // fd02 -> fd 02
    // fd03 -> fd 03
    // fdfc -> fd fc
    // fdfd -> fd fd
    // fdfe -> fd fe
    // fdff -> fd ff

    // 0xfe00, 0xfe01, 0xfe02, 0xfe03, 0xfefc, 0xfefd, 0xfefe, 0xfeff,
    // fe00 -> 01 fe
    // fe01 -> fe 01
    // fe02 -> fe 02
    // fe03 -> fe 03
    // fefc -> fe fc
    // fefd -> fe fd
    // fefe -> fe fe
    // feff -> fe ff

    // 0xff00, 0xff01, 0xff02, 0xff03, 0xfffc, 0xfffd, 0xfffe, 0xffff 
    // ff00 -> 01 ff
    // ff01 -> ff 01
    // ff02 -> ff 02
    // ff03 -> ff 03
    // fffc -> ff fc
    // fffd -> ff fd
    // fffe -> ff fe
    // ffff -> ff ff
    };


void COBSRLoopCheck( void ){
    

    //for( int i = 0; i < sizeof( oneByteArray); i++ ){
    //    COBSRCheck( &oneByteArray[i], 1 );
    //}
    for( int i = 0; i < sizeof( twoByteArray) / sizeof( uint16_t ); i++ ){
        COBSRCheck( &twoByteArray[i], 2 );
    }
}
