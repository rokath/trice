
#include "trice.h"
#include "cobsr.h"

//! COBSREncode does the same as the cobsr_encode function but a bit faster.
//! by leaving out some checks, assuming max 254 source bytes and uses a simpler signature.
//! See https://github.com/ctrl-labs/cobs-c for more details.
//! \param dst is the result buffer. It must be at least 1 byte longer than len.
//! \param src is the source buffer.
//! \param len is count of valid data inside the the source buffer. Assumption: 0 < len < 255.
//! \retval is the count of valid data inside the the result buffer. It is len or len+1.
size_t COBSREncode(uint8_t *dst, const uint8_t * src, size_t len){
    const uint8_t* limit = src + len; // end of source 
    uint8_t*       code  = dst;       // next code position
    uint8_t*       data  = dst + 1;   // next data position
    uint8_t        leg   = 1;         // search length
    uint8_t        by;                // byte
    for (;;) { // Iterate over the source bytes
        by = *src++;
        if (by == 0) { // We found a zero byte
            *code = leg;
            code = data++;
            leg = 1;
            if (src >= limit){
                break;
            }
        } else { // Copy the non-zero byte to the destination buffer
            *data++ = by;
            leg++;
            if (src >= limit){
                break;
            }
        }
    }
    // We've reached the end of the source data (or possibly run out of output buffer)
    // Finalise the remaining output. In particular, write the code (length) byte.
    //
    // For COBS/R, the final code (length) byte is special: if the final data byte is
    // greater than or equal to what would normally be the final code (length) byte,
    // then replace the final code byte with the final data byte, and remove the final
    // data byte from the end of the sequence. This saves one byte in the output.
    //
    // Update the pointer to calculate the final output length.
    if (by < leg){ // Encoding same as plain COBS 
        *code = leg;
    } else { // Special COBS/R encoding: length code is final byte, and final byte is removed from data sequence.
        *code = by;
        return len; // data--;
    }
    return len+1; //data - dst; // Calculate the output length, from the value of code
}

//! COBSREncodeWithShortRules does the same as the COBSREncode function but a bit faster for len 1,2,3,4 when first 2 bytes both !=0.
//! \param dst is the result buffer. It must be at least 1 byte longer than len.
//! \param src is the source buffer.
//! \param len is count of valid data inside the the source buffer.
//! \retval is the count of valid data inside the the result buffer.
size_t COBSREncodeWithShortRules(uint8_t *dst, const uint8_t *src, size_t len){
    switch( len ){
        #if 1
        case 1: { // 52 ticks instead of 49
            // Only case 0 and 1 is to be treated.
            // 00 -> 01 01
            // 01 -> 02 01
            // 02 -> 02
            // 03 -> 03
            // fc -> fc
            // fd -> fd
            // fe -> fe
            // ff -> ff
            uint8_t tv = *src;
            if (tv < 2 ) {
                dst[0] = tv + 1;
                dst[1] = 1;
                return 2;
            }else{ // just copy
                dst[0] = tv;
                return 1;
            }
        }
        case 2: // 62 ticks instead of 69
            // just swap in the assumtion src[0] > 3 or src[1] != 0
            // The first 2 bytes are assumed to be ii > 0x03 && ic != 0 each. They used for the ID plus cycle counter.
            // This way the COBSR encoded buffer is just the swapped original.
            // ii ic -> ii ic
            // -----------------------
            // 0000 -> 01 01 01 (excluded)
            // 0001 -> 02 01 01 (excluded)
            // 0002 -> 02 02 01 (excluded)
            // 0003 -> 02 03 01 (excluded)
            // 00fc -> 02 fc 01 (excluded)
            // 00fd -> 02 fd 01 (excluded)
            // 00fe -> 02 fe 01 (excluded)
            // 00ff -> 02 ff 01 (excluded)
            //
            // 0100 -> 01 02 01 (excluded)
            // 0101 -> 03 01 01 (excluded)
            // 0102 -> 03 02 01 (excluded)
            // 0103 -> 03 03 01 (excluded)
            // 01fc -> 03 fc 01 (excluded)
            // 01fd -> 03 fd 01 (excluded)
            // 01fe -> 03 fe 01 (excluded)
            // 01ff -> 03 ff 01 (excluded)
            // 
            // 0200 -> 01 02    (excluded)
            // 0201 -> 03 01 02 (excluded)
            // 0202 -> 03 02 02 (excluded)
            // 0203 -> 03 03 02 (excluded)
            // 02fc -> 03 fc 02 (excluded)
            // 02fd -> 03 fd 02 (excluded)
            // 02fe -> 03 fe 02 (excluded)
            // 02ff -> 03 ff 02 (excluded)
            //
            // 0300 -> 01 03    (excluded)
            // 0301 -> 03 01
            // 0302 -> 03 02
            // 0303 -> 03 03
            // 03fc -> 03 fc
            // 03fd -> 03 fd
            // 03fe -> 03 fe
            // 03ff -> 03 ff
            //
            // fc00 -> 01 fc    (excluded)
            // fc01 -> fc 01
            // fc02 -> fc 02
            // fc03 -> fc 03
            // fcfc -> fc fc
            // fcfd -> fc fd
            // fcfe -> fc fe
            // fcff -> fc ff
            //
            // fd00 -> 01 fd    (excluded)
            // fd01 -> fd 01
            // fd02 -> fd 02
            // fd03 -> fd 03
            // fdfc -> fd fc
            // fdfd -> fd fd
            // fdfe -> fd fe
            // fdff -> fd ff
            //
            // fe00 -> 01 fe    (excluded)
            // fe01 -> fe 01
            // fe02 -> fe 02
            // fe03 -> fe 03
            // fefc -> fe fc
            // fefd -> fe fd
            // fefe -> fe fe
            // feff -> fe ff
            //
            // ff00 -> 01 ff    (excluded)
            // ff01 -> ff 01
            // ff02 -> ff 02
            // ff03 -> ff 03
            // fffc -> ff fc
            // fffd -> ff fd
            // fffe -> ff fe
            // ffff -> ff ff
            if( src[0] <= 3 || src[1] == 0 ){
                return COBSREncode( dst, src, len );
            }
            dst[0] = src[1];
            dst[1] = src[0];
            return 2;
        case 3: { // 67 ticks instead of 89
            // The first 2 bytes (ii ic) are assumed to be != 0 each. They used for the ID plus cycle counter.
            // ii ic vv -> vv ii ic
            // -----------------------
            // 11 1c fe -> fe 11 1c
            // 11 1c ff -> ff 11 1c
            // 11 1c 00 -> 03 11 1c 01
            // 11 1c 01 -> 04 11 1c 01
            // 11 1c 02 -> 04 11 1c 02
            // 11 1c 03 -> 04 11 1c 03
            // 11 1c 04 -> 04 11 1c
            // 11 1c 05 -> 05 11 1c
            //
            // Derived rules:
            // if       ( vv == 0 ){ write 03 ii ic 01
            // } else if( vv < 4)  { write 04 ii ic vv
            // } else              { write vv ii ic
            uint8_t vv;
            dst[1] = src[0]; // ii
            dst[2] = src[1]; // ic
            vv = src[2];
            if( vv > 3 ){
                dst[0] = vv;
                return 3;
            }else if ( vv == 0 ) {
                dst[0] = 0x03;
                dst[3] = 0x01;
                return 4;
            }else{
                dst[0] = 0x04;
                dst[3] = vv;
                return 4;
            }
        }
        case 4: { // 94 ticks instead of 109
            // The first 2 bytes (ii ic) are assumed to be != 0 each. They used for the ID plus cycle counter.
            // ll lc v0 v1
            // --------------------------
            // 11 1c fe aa -> aa 11 1c fe
            // 11 1c ff aa -> aa 11 1c ff
            // 11 1c 00 aa -> 03 11 1c aa
            // 11 1c 01 aa -> aa 11 1c 01
            //
            // ll lc v0 v1
            // 11 1c aa fe -> fe 11 1c aa
            // 11 1c aa ff -> ff 11 1c aa
            // 11 1c aa 00 -> 04 11 1c aa 01
            // 11 1c aa 01 -> 05 11 1c aa 01
            // 11 1c aa 02 -> 05 11 1c aa 02
            // 11 1c aa 03 -> 05 11 1c aa 03
            // 11 1c aa 04 -> 05 11 1c aa 04
            // 11 1c aa 05 -> 05 11 1c aa
            // 11 1c aa 06 -> 06 11 1c aa
            //
            // 11 1c fe 00 -> 04 11 1c fe 01
            // 11 1c ff 00 -> 04 11 1c ff 01
            // 11 1c 00 00 -> 03 11 1c 01 01 
            // 11 1c 01 00 -> 04 11 1c 01 01
            // 11 1c 02 00 -> 04 11 1c 02 01
            //
            // Derived rules:
            //        if( v0 == 00 && v1 != 00 ) { write 03 11 1c v1 
            // } else if( v0 == 00 && v1 == 00 ) { write 03 11 1c 01 01
            // } else if( v0 != 00 && v1 == 00 ) { write 04 11 1c aa 01
            // } else if( v0 != 00 && v1  <  5 ) { write 05 ll lc v0 v1
            // } else                            { write v1 ll lc v0
            // }
            uint8_t v0;
            uint8_t v1;
            dst[1] = src[0];
            dst[2] = src[1];
            v0 = src[2];
            v1 = src[3];
            if( v0 == 0 && v1 == 0 ){       // write 03 11 1c 01 01
                dst[0] = 0x03;
                dst[3] = 0x01;
                dst[4] = 0x01;
                return 5;                
            } else if( v0 == 0 && v1 != 0 ){ // write 03 11 1c v1 
                dst[0] = 0x03;
                dst[3] = v1;
                return 4;
            } else if( v0 != 0 && v1 == 0 ){ // write 04 11 1c aa 01
                dst[0] = 0x04;
                dst[3] = v1;
                dst[4] = 0x01;
                return 5;
            } else if( v0 != 0 && v1  <  5 ){ // write 05 ll lc v0 v1   
                dst[0] = 0x05;
                dst[3] = v0;
                dst[4] = v1;
                return 5;
            } else {                          // write v1 ll lc v0
                dst[0] = v1;
                dst[3] = v0;
                return 4;
            }
        }
        #endif
        default:
            return COBSREncode( dst, src, len );
    }
}

//! COBSRCheck12 is an internel helper for displaying COBSR data
static void COBSRCheck12( void* pTestValue, int byteWidth ){

    cobsr_decode_result COBSDecResult = {0};
    uint8_t enc[12], dec[12];
    size_t len;

    switch( byteWidth ){ // print left side
        case 1: TRICE16( Id( 47229), "msg:%02x -> ", *(uint8_t*)pTestValue ); break;
        case 2: TRICE16( Id( 36614), "msg:%04x -> ", *(uint16_t*)pTestValue ); break;
        default: return;
    }
    
    len = COBSREncode(enc, pTestValue, byteWidth);

    // print right side
    switch( len ){
        case 1: TRICE16( Id( 37961), "sig:%02x\n", enc[0]); break;
        case 2: TRICE16( Id( 41310), "sig:%02x %02x\n", enc[0], enc[1]); break;
        case 3: TRICE16( Id( 32787), "sig:%02x %02x %02x\n", enc[0], enc[1], enc[2]); break;
        case 4: TRICE16( Id( 37461), "sig:%02x %02x %02x %02x\n", enc[0], enc[1], enc[2], enc[3]); break;
    }

    COBSDecResult = cobsr_decode(
        dec,             // void * dst_buf_ptr, 
        sizeof( dec ),   // size_t dst_buf_len,
        enc,             // const void * src_ptr,
        len);                   // size_t src_len);
    
    if( COBSR_DECODE_OK != COBSDecResult.status ){
        TRICE16( Id( 35623), "err: i = %d, COBSDecResult.status = %d\n", 0, COBSDecResult.status );
    }else{
        if( (byteWidth != COBSDecResult.out_len)  ){
            TRICE16( Id( 53032), "err: COBSDecResult.out_len = %d != byteWidth = %x\n", COBSDecResult.out_len, byteWidth);
            return;
        }
        switch( byteWidth ) { // check backward step
            case 1: {  
            uint8_t tv = *(uint8_t*)pTestValue;              
                if( tv != dec[0] ){
                    TRICE8_2( Id( 45258), "err:pTestValue %02x != dec[0] = %02x\n", tv, dec[0] );
                }
            }
            return;
            case 2: {
                uint16_t tv = *(uint16_t*)pTestValue;
                if( tv != *(uint16_t*)dec ){
                TRICE16_2( Id( 54626), "err:pTestValue %04x != dec[0] = %04x\n", tv, *(uint16_t*)dec );
                }
            }
            return;
        }
    }
}


// COBSREncodingCheck1 writes 1 byte COBSR encoding to find fast encoding rules.
// Only case 0 and 1 is to be treated.
// 00 -> 01 01
// 01 -> 02 01
// 02 -> 02
// 03 -> 03
// fc -> fc
// fd -> fd
// fe -> fe
// ff -> ff
void COBSREncodingCheck1( void ){
    static uint8_t oneByteArray[] = {
    0,      1,      2,      3,   0xFC,   0xFD,   0xFE,   0xFF 
    };
    for( int i = 0; i < sizeof( oneByteArray); i++ ){
        COBSRCheck12( &oneByteArray[i], 1 );
    }
}

//! COBSREncodingCheck2 writes 2 byte COBSR encoding to find fast encoding rules.
//! The first 2 bytes are assumed to be ii > 0x03 && ic != 0 each. They used for the ID plus cycle counter.
//! This way the COBSR encoded buffer is identical to the original.
//! ii ic -> ii ic
//! -----------------------
//! 0000 -> 01 01 01 (excluded)
//! 0001 -> 02 01 01 (excluded)
//! 0002 -> 02 02 01 (excluded)
//! 0003 -> 02 03 01 (excluded)
//! 00fc -> 02 fc 01 (excluded)
//! 00fd -> 02 fd 01 (excluded)
//! 00fe -> 02 fe 01 (excluded)
//! 00ff -> 02 ff 01 (excluded)
//!
//! 0100 -> 01 02 01 (excluded)
//! 0101 -> 03 01 01 (excluded)
//! 0102 -> 03 02 01 (excluded)
//! 0103 -> 03 03 01 (excluded)
//! 01fc -> 03 fc 01 (excluded)
//! 01fd -> 03 fd 01 (excluded)
//! 01fe -> 03 fe 01 (excluded)
//! 01ff -> 03 ff 01 (excluded)
//! 
//! 0200 -> 01 02    (excluded)
//! 0201 -> 03 01 02 (excluded)
//! 0202 -> 03 02 02 (excluded)
//! 0203 -> 03 03 02 (excluded)
//! 02fc -> 03 fc 02 (excluded)
//! 02fd -> 03 fd 02 (excluded)
//! 02fe -> 03 fe 02 (excluded)
//! 02ff -> 03 ff 02 (excluded)
//!
//! 0300 -> 01 03    (excluded)
//! 0301 -> 03 01
//! 0302 -> 03 02
//! 0303 -> 03 03
//! 03fc -> 03 fc
//! 03fd -> 03 fd
//! 03fe -> 03 fe
//! 03ff -> 03 ff
//!
//! fc00 -> 01 fc    (excluded)
//! fc01 -> fc 01
//! fc02 -> fc 02
//! fc03 -> fc 03
//! fcfc -> fc fc
//! fcfd -> fc fd
//! fcfe -> fc fe
//! fcff -> fc ff
//!
//! fd00 -> 01 fd    (excluded)
//! fd01 -> fd 01
//! fd02 -> fd 02
//! fd03 -> fd 03
//! fdfc -> fd fc
//! fdfd -> fd fd
//! fdfe -> fd fe
//! fdff -> fd ff
//!
//! fe00 -> 01 fe    (excluded)
//! fe01 -> fe 01
//! fe02 -> fe 02
//! fe03 -> fe 03
//! fefc -> fe fc
//! fefd -> fe fd
//! fefe -> fe fe
//! feff -> fe ff
//!
//! ff00 -> 01 ff    (excluded)
//! ff01 -> ff 01
//! ff02 -> ff 02
//! ff03 -> ff 03
//! fffc -> ff fc
//! fffd -> ff fd
//! fffe -> ff fe
//! ffff -> ff ff
void COBSREncodingCheck2( void ){
    static uint16_t twoByteArray[] = { 
        0x0000, 0x0001, 0x0002, 0x0003, 0x00fc, 0x00fd, 0x00fe, 0x00ff,
        0x0100, 0x0101, 0x0102, 0x0103, 0x01fc, 0x01fd, 0x01fe, 0x01ff,  
        0x0200, 0x0201, 0x0202, 0x0203, 0x02fc, 0x02fd, 0x02fe, 0x02ff,
        0x0300, 0x0301, 0x0302, 0x0303, 0x03fc, 0x03fd, 0x03fe, 0x03ff,
        0xfc00, 0xfc01, 0xfc02, 0xfc03, 0xfcfc, 0xfcfd, 0xfcfe, 0xfcff,
        0xfd00, 0xfd01, 0xfd02, 0xfd03, 0xfdfc, 0xfdfd, 0xfdfe, 0xfdff,
        0xfe00, 0xfe01, 0xfe02, 0xfe03, 0xfefc, 0xfefd, 0xfefe, 0xfeff,
        0xff00, 0xff01, 0xff02, 0xff03, 0xfffc, 0xfffd, 0xfffe, 0xffff,
    };
    for( int i = 0; i < sizeof( twoByteArray) / sizeof( uint16_t ); i++ ){
        COBSRCheck12( &twoByteArray[i], 2 );
    }  
}

//! COBSREncodingCheck3 writes 3 byte COBSR encoding to find fast encoding rules.
//! The first 2 bytes (ii ic) are assumed to be != 0 each. They used for the ID plus cycle counter.
//! ii ic vv -> vv ii ic
//! -----------------------
//! 11 1c fe -> fe 11 1c
//! 11 1c ff -> ff 11 1c
//! 11 1c 00 -> 03 11 1c 01
//! 11 1c 01 -> 04 11 1c 01
//! 11 1c 02 -> 04 11 1c 02
//! 11 1c 03 -> 04 11 1c 03
//! 11 1c 04 -> 04 11 1c
//! 11 1c 05 -> 05 11 1c
//! Derived rules:
//! if       ( vv == 0 ){ write 03 ii ic 01
//! } else if( vv < 4)  { write 04 ii ic vv
//! } else              { write vv ii ic
void COBSREncodingCheck3( void ){
    static uint8_t idx = 0xfe;
    size_t dlen;
    size_t len = 3;   
    uint8_t src[3];
    uint8_t dst[4];
    src[0] = 0x11;
    src[1] = 0x1c;
    src[2] = idx;
    dlen = COBSREncode( dst, src, len );
    trice8( Id( 39801), "msg:%02x %02x %02x -> %02x %02x %02x ", src[0], src[1], src[2], dst[0], dst[1], dst[2] );
    if( 4 == dlen ){
        trice8( Id( 49285), "msg:%02x\n", dst[3] );
    } else {
        trice0( Id( 43024), "msg:\n" );
    }
    idx++;
}    

//! COBSREncodingCheck4 writes 4 byte COBSR encoding to find fast encoding rules.
//! The first 2 bytes (ii ic) are assumed to be != 0 each. They used for the ID plus cycle counter.
//! ll lc v0 v1
//! 11 1c fe aa -> aa 11 1c fe
//! 11 1c ff aa -> aa 11 1c ff
//! 11 1c 00 aa -> 03 11 1c aa
//! 11 1c 01 aa -> aa 11 1c 01
//!
//! ll lc v0 v1
//! 11 1c aa fe -> fe 11 1c aa
//! 11 1c aa ff -> ff 11 1c aa
//! 11 1c aa 00 -> 04 11 1c aa 01
//! 11 1c aa 01 -> 05 11 1c aa 01
//! 11 1c aa 02 -> 05 11 1c aa 02
//! 11 1c aa 03 -> 05 11 1c aa 03
//! 11 1c aa 04 -> 05 11 1c aa 04
//! 11 1c aa 05 -> 05 11 1c aa
//! 11 1c aa 06 -> 06 11 1c aa
//!
//! 11 1c fe 00 -> 04 11 1c fe 01
//! 11 1c ff 00 -> 04 11 1c ff 01
//! 11 1c 00 00 -> 03 11 1c 01 01 
//! 11 1c 01 00 -> 04 11 1c 01 01
//! 11 1c 02 00 -> 04 11 1c 02 01
//! Derived rules:
//!        if( v0 == 00 && v1 != 00 ) { write 03 11 1c v1 
//! } else if( v0 == 00 && v1 == 00 ) { write 03 11 1c 01 01
//! } else if( v0 != 00 && v1 == 00 ) { write 04 11 1c aa 01
//! } else if( v0 != 00 && v1  <  5 ) { write 05 ll lc v0 v1
//! } else                            { write v1 ll lc v0
//! }
void COBSREncodingCheck4( void ){
    size_t dlen;
    size_t len = 4;
    static uint8_t idx = 0;
    uint8_t src[4];
    uint8_t dst[5];
    src[0] = 0x11;
    src[1] = 0x1c;
    src[2] = idx;
    src[3] = 0x00;
    dlen = COBSREncode( dst, src, len );
    trice8( Id( 63981), "msg:%02x %02x %02x %02x -> %02x %02x %02x %02x ", src[0], src[1], src[2], src[3], dst[0], dst[1], dst[2], dst[3] );
    if( 5 == dlen ){
        trice8( Id( 49285), "msg:%02x\n", dst[4] );
    } else {
        trice0( Id( 43024), "msg:\n" );
    }
    idx++;
}    

// compareArrays is a helper function for tests
static int compareArrays(uint8_t a[], uint8_t b[], int n) {
  int i;
  for(i = 0; i < n; i++) {
    if (a[i] != b[i]) return 0;
  }
  return 1;
}

//! COBSREncodingCheck128 uses random arrays of len 1...128 with random content, COBSR encodes them with the rewritten COBSREncode
//! function and compares after cobsr_decode.
void COBSREncodingCheck128( void ){
    static uint8_t src[200];
    static uint8_t dst[200];
    static uint32_t longer[200] = {0};
    static uint32_t equal[200] = {0};
    static uint8_t dec[200];
    size_t dlen;
    static size_t len = 1; // random: 1 <= len <= 128
 
    for( int i = 0; i < len; i++ ){
        src[i] = 0xff & SYSTICKVAL16; // fill with random bytes
    }
    if( len < 19 || len%8 == 0 ){ // with time measure
        uint16_t tick1, tick0 = SYSTICKVAL16; // backward counter
        dlen = COBSREncodeWithShortRules( dst, src, len );
        tick1 = SYSTICKVAL16;
        trice16( Id( 55325), "time:len=%3u, ticks=%5d, equal=%5u, longer=%5u\n", len, tick0-tick1, equal[len], longer[len] );
    }else{
        dlen = COBSREncodeWithShortRules( dst, src, len );
    }
    if( dlen == len + 1 ){
        longer[len]++;
    }else if( dlen == len ){
        equal[len]++;
    } else {
        trice32( Id( 62077), "err:dlen=%u, len=%u\n", dlen, len );
    }
    {
        cobsr_decode_result COBSDecResult = {0};
        COBSDecResult = cobsr_decode(
            dec,         // void * dst_buf_ptr, 
            sizeof(dec), // size_t dst_buf_len,
            dst,         // const void * src_ptr,
            dlen);       // size_t src_len);
    
        if( COBSR_DECODE_OK != COBSDecResult.status ){
            TRICE16( Id( 35623), "err: i = %d, COBSDecResult.status = %d\n", 0, COBSDecResult.status );
            return;
        }else if( len != COBSDecResult.out_len ){
            TRICE16( Id( 33509), "err: COBSDecResult.out_len = %d != len = %x\n", COBSDecResult.out_len, len);
            return;
        }
    }
    if( !compareArrays( src, dec, len ) ){
        trice8( Id( 49088), "wrn:len=%u\n", len );
        for( int i = 0; i < len; i++ ){
            trice8( Id( 58853), "err:%02x ? %02x\n", src[i], dec[i] );
        }
        return;
    }
    len = (0 < len && len < 128) ? len + 1 : 1;
}


//  //! Wrap_cobsr_encode calls the original cobsr_encode function but has a simpler sinature.
//  //! It is usable for verifying.
//  size_t Wrap_cobsr_encode(uint8_t *dst, const uint8_t * src, size_t len){
//      cobsr_encode_result COBSEncResult = cobsr_encode( 
//      dst,           // void * dst_buf_ptr, 
//      257,           // size_t dst_buf_len,
//      src,           // const void * src_ptr, 
//      len );         // size_t src_len)
//      return COBSEncResult.out_len;
//  }

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
