/*! \file trice.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#include "trice.h"

//! triceCommand is the command receive buffer.
char triceCommand[TRICE_COMMAND_SIZE_MAX+1]; // with terminating 0

//! triceCommandFlag ist set, when a command was received completely.
int triceCommandFlag = 0; // updated

static unsigned triceDepthMax = 0; //!< triceDepthMax is a diagnostics value usable to optimize buffer size.

#if TRICE_CYCLE_COUNTER == 1
uint8_t  TriceCycle = 0xc0; //!< TriceCycle is increased and transmitted with each trice message, if enabled.
#endif

#ifdef TRICE_HALF_BUFFER_SIZE
static uint32_t triceBuffer[2][TRICE_HALF_BUFFER_SIZE>>2] = {0}; //!< triceBuffer is a double buffer for better write speed.
static int triceSwap = 0; //!< triceSwap is the index of the active write buffer. !triceSwap is the active read buffer index.
    uint32_t* TriceBufferWritePosition = &triceBuffer[0][TRICE_DATA_OFFSET>>2]; //!< TriceBufferWritePosition is the active write position.
static uint32_t* triceBufferWriteLimit = &triceBuffer[1][TRICE_DATA_OFFSET>>2]; //!< triceBufferWriteLimit is the triceBuffer written limit. 

#if defined( TRICE_UART ) && defined( TRICE_HALF_BUFFER_SIZE ) // buffered out to UART
static int triceNonBlockingWrite( void const * buf, int nByte );
#define TRICE_WRITE( buf, len ) do{ triceNonBlockingWrite( buf, len ); }while(0)
#endif

//! triceBufferSwap swaps the trice double buffer and returns the read buffer address.
static uint32_t* triceBufferSwap( void ){
    TRICE_ENTER_CRITICAL_SECTION
    triceBufferWriteLimit = TriceBufferWritePosition; // keep end position
    triceSwap = !triceSwap; // exchange the 2 buffers
    TriceBufferWritePosition = &triceBuffer[triceSwap][TRICE_DATA_OFFSET>>2]; // set write position for next TRICE
    TRICE_LEAVE_CRITICAL_SECTION
    return &triceBuffer[!triceSwap][0];
}

//! triceDepth returns the total trice byte count ready for transfer.
//! The trice data start at tb + TRICE_DATA_OFFSET.
//! The returned depth is without the TRICE_DATA_OFFSET offset.
static size_t triceDepth( uint32_t* tb ){
    size_t depth = (triceBufferWriteLimit - tb)<<2; // 32-bit write width
    return depth - TRICE_DATA_OFFSET;
}

//! TriceTransfer, if possible, swaps the double buffer and initiates a write.
//! It is the resposibility of the app to call this function once every 10-100 milliseconds.
void TriceTransfer( void ){
    if( 0 == TriceOutDepth() ){ // transmission done, so a swap is possible
        uint32_t* tb = triceBufferSwap(); 
        size_t tLen = triceDepth(tb); // tlen is always a multiple of 4
        if( tLen ){
            TriceOut( tb, tLen );
        }
    } // else: transmission not done yet
}

//! TriceDepthMax returns the max trice buffer depth until now.
size_t TriceDepthMax( void ){
    size_t currentDepth = 4*(TriceBufferWritePosition - &triceBuffer[triceSwap][0]); 
    return currentDepth > triceDepthMax ? currentDepth : triceDepthMax;
}

#else // #ifdef TRICE_HALF_BUFFER_SIZE

//! TriceDepthMax returns the max trice buffer depth until now.
size_t TriceDepthMax( void ){
    return triceDepthMax;
}

#endif // #else #ifdef TRICE_HALF_BUFFER_SIZE


#if TRICE_ENCODING == TRICE_LEGACY_ENCODING

void TriceOut( uint32_t* tb, size_t tLen ){
    size_t eLen, cLen;
    uint8_t* co = (uint8_t*)tb; // encoded COBS data starting address
    uint32_t* da = tb + (TRICE_DATA_OFFSET>>2)-1; // start of unencoded COBS package data: descriptor and trice data
    *da = TRICE_COBS_PACKAGE_MODE; // add a 32-bit COBS package mode descriptor in front of trice data. That allowes to inject third-party non-trice COBS packages.
    eLen = tLen + 4; // add COBS package mode descriptor length 
    #ifdef TRICE_ENCRYPT
    eLen = (eLen + 4) & ~7; // only multiple of 8 encryptable
    TriceEncrypt( da, eLen>>2 );
    #endif
    cLen = TriceCOBSEncode(co, (uint8_t*)da, eLen);
    do{                 // Add 1 to 4 zeroes as COBS package delimiter.
        co[cLen++] = 0; // One is ok, but padding to an uint32_t border could make TRICE_WRITE faster.
    }while( cLen & 3 ); // Additional empty packages are ignored on th receiver side.
    TRICE_WRITE( co, cLen );
    tLen += TRICE_DATA_OFFSET; 
    triceDepthMax = tLen < triceDepthMax ? triceDepthMax : tLen; // diagnostics
}

#else // #if TRICE_ENCODING == TRICE_LEGACY_ENCODING

//! triceDataLen returns encoded len.
//! \param p points to nc
// *da = 11iiiiiiI 11iiiiiiI TT TT NC ...
// *da = 10iiiiiiI TT NC ...
// *da = 01iiiiiiI 11iiiiiiI NC ...
// *da = 00...
static size_t triceDataLen( uint8_t* p ){
    uint16_t nc = *(uint16_t*)p;
    size_t n = nc>>8;
    if( n < 128 ){
        return n;
    }
    return nc & 0x7fff;
}

unsigned triceErrorCount = 0;

//! nextTrice expects at *buf 32-bit aligned trice messages and returns the next one in pStart and pLen.
//! *buf is filled with the advanced buf and *pSize gets the reduced value.
//! \retval is 0 on success or negative on error.
static int nextTrice( uint8_t** buf, size_t* pSize, uint8_t** pStart, size_t* pLen ){
    uint16_t* pNC = (uint16_t*)*buf;
    int triceType = *pNC >> 14;
    int offset = 0;
    size_t size = *pSize;
    size_t triceSize;
    size_t len; 
    *pStart = *buf;
    switch( triceType ){
        case 0: // EX
            len = size; // todo: Change that when needed.
            // Extended trices without length information cannot be separated here.
            // But it is possible to store them with length information and to remove it here.
            break;
        case 1: // T0
            len = 4 + triceDataLen(*pStart + 2); // tyId
            break;
        case 2: // T2
            *pStart += 2; // see Id(n) macro definition
            offset = 2;
            len = 6 + triceDataLen(*pStart + 4); // tyId ts16
            break;
        case 3: // T4
            len = 8 + triceDataLen(*pStart + 6); // tyId ts32
            break;
    }
    triceSize = (len + offset + 3) & ~3;
    // T2 case example:             triceSize  len   t-0-3   t-o
    // 80id 80id 1616 00cc                8     6      3      6
    // 80id 80id 1616 01cc dd            12     7      7     10
    // 80id 80id 1616 02cc dd dd         12     8      7     10
    // 80id 80id 1616 03cc dd dd dd      12     9      7     10
    // 80id 80id 1616 04cc dd dd dd dd   12    10      7     10
    if( !( triceSize - offset - 3 <= len && len <= triceSize - offset )){ // corrupt data
        triceErrorCount++;
        return -__LINE__;
    }    
    size -= triceSize;
    *buf += triceSize;
    *pSize = size;
    *pLen = len;
    return 0;
}

static size_t triceEncode( uint8_t* enc, uint8_t* buf, size_t len ){
    size_t encLen;
    #ifdef TRICE_ENCRYPT
    len = (len + 7) & ~7; // only multiple of 8 encryptable
    TriceEncrypt( enc + TRICE_DATA_OFFSET, len>>2 );
    #endif
    #if TRICE_PACKAGE_FRAMING == TRICE_COBS_FRAMING
    {
        unsigned TriceCOBSEncode( uint8_t* restrict output, const uint8_t * restrict input, unsigned length);
        encLen = TriceCOBSEncode(enc, buf, len);
    }
    #else
    {
        int TCOBSEncode( void * restrict output, const void * restrict input, size_t length);
        encLen = TCOBSEncode(enc, buf, len);
    }
    #endif
    enc[encLen++] = 0; // Add zero as package delimiter.
    return encLen;
}

//! TriceOut encodes trices and writes them in one step to the output.
//! \param tb is start of uint32_t* trice buffer. The space TRICE_DATA_OFFSET at
//! the tb start is for in-buffer encoding of the trice data.
//! \param tLen is length of trice data. tlen is always a multiple of 4 because
//! of 32-bit alignment and padding bytes.
void TriceOut( uint32_t* tb, size_t tLen ){
    uint8_t* enc = (uint8_t*)tb; // encoded data starting address
    size_t encLen = 0;
    uint8_t* buf = enc + TRICE_DATA_OFFSET; // start of 32-bit aligned trices
    size_t len = tLen; // (byte count)
    // diagnostics
    tLen += TRICE_DATA_OFFSET; 
    triceDepthMax = tLen < triceDepthMax ? triceDepthMax : tLen;
    while(len){
        uint8_t* triceStart;
        size_t triceLen;
        int r = nextTrice( &buf, &len, &triceStart, &triceLen );
        if( r < 0 ){ // on data error
            break;   // ignore following data
        }
        #if TRICE_TRANSFER_MODE == TRICE_SAFE_SINGLE_MODE
        encLen += triceEncode( enc+encLen, triceStart, triceLen );
        #endif
        #if  TRICE_TRANSFER_MODE == TRICE_PACK_MULTI_MODE
        memmove(enc + TRICE_DATA_OFFSET + encLen, triceStart, triceLen );
        encLen += triceLen;
        #endif
    }
    #if  TRICE_TRANSFER_MODE == TRICE_PACK_MULTI_MODE
    encLen = triceEncode( enc, enc + TRICE_DATA_OFFSET, encLen);
    #endif
    TRICE_WRITE( enc, encLen );
}

#endif // #else // #if TRICE_ENCODING == TRICE_LEGACY_ENCODING

#if defined( TRICE_UART ) && !defined( TRICE_HALF_BUFFER_SIZE ) // direct out to UART
//! triceBlockingPutChar returns after c was successfully written.
static void triceBlockingPutChar( uint8_t c ){
    while( !triceTxDataRegisterEmpty() );
    triceTransmitData8( c );
} 

//! TriceBlockingWrite returns after buf was completely written.
void TriceBlockingWrite( uint8_t const * buf, unsigned len ){
    for( unsigned i = 0; i < len; i++ ){ 
        triceBlockingPutChar( buf[i] ); }
}
#endif // #if defined( TRICE_UART ) && !defined( TRICE_HALF_BUFFER_SIZE )

#if defined( TRICE_UART ) && defined( TRICE_HALF_BUFFER_SIZE ) // buffered out to UART
static uint8_t const * triceOutBuffer;
static int triceOutCount = 0;
static int triceOutIndex = 0;

//! triceNonBlockingWrite
static int triceNonBlockingWrite( void const * buf, int nByte ){
    triceOutBuffer = buf;
    triceOutIndex = 0;
    triceOutCount = nByte;
    return nByte;
}

//! TriceOutDepth returns the amount of bytes not written yet.
int TriceOutDepth( void ){
    return triceOutCount - triceOutIndex;
}

//! TriceNextUint8 returns the next trice byte for transmission.
uint8_t TriceNextUint8( void ){
    return triceOutBuffer[triceOutIndex++];
}

//! triceServeTransmit must be called cyclically to proceed ongoing write out.
//! A good place is UART ISR.
void triceServeTransmit(void) {
    triceTransmitData8(TriceNextUint8());
    if (0 == TriceOutDepth()) { // no more bytes
        triceDisableTxEmptyInterrupt();
    }
}

// triceTriggerTransmit must be called cyclically to initialize write out.
void triceTriggerTransmit(void){
    if( TriceOutDepth() && triceTxDataRegisterEmpty() ){
        triceEnableTxEmptyInterrupt(); // next bytes
    }
}
#endif // #if defined( TRICE_UART ) && defined( TRICE_HALF_BUFFER_SIZE )

#ifdef TRICE_ENCRYPT // needs a re-design
//! golang XTEA works with 64 rounds
static const unsigned int numRounds = 64;

//! 128 bit static key
static const uint32_t k[4] = TRICE_ENCRYPT; 

//! internal constant
static const uint32_t delta = 0x9E3779B9;

//! precomputed values for faster execution
static uint32_t table[64];

//! TriceInitXteaTable precalculates the table.
//! It is possible to put this table completely into FLASH by precomputing it during compile time.
void TriceInitXteaTable( void ){
    uint32_t sum = 0;
    // Two rounds of XTEA applied per loop
    for( int i = 0; i < numRounds; ) {
        table[i] = sum + k[sum&3];
        i++;
        sum += delta;
        table[i] = sum + k[(sum>>11)&3];
        i++;
    }
}

// encipher converts 64 bits.
//! Code taken and adapted from xtea\block.go
//!\param v 64 bits of data in v[0] and v[1] are encoded in place
static void encipher( uint32_t v[2] ) {
    uint32_t v0 = v[0], v1 = v[1];
    for( int i=0; i < numRounds; ) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ table[i];
        i++;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ table[i];
        i++;
    }
    v[0] = v0; v[1] = v1;
}

#ifdef TRICE_DECRYPT
//! decipher reverses encipher action.
//! Code taken and adapted from xtea\block.go
//!\param v 64 bits of data in v[0] and v[1] are decoded in place
static void decipher( uint32_t v[2] ) {
    uint32_t v0 = v[0], v1 = v[1];
    for( int i=numRounds; i > 0; ) {
        i--;
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ table[i];
        i--;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ table[i];
    }
    v[0] = v0; v[1] = v1;
}


//! TriceDecrypt re-converts from xtea cipher.
//! \param p pointer to 8 byte buffer
void TriceDecrypt( uint32_t* p, unsigned count ){
    for( int i = 0; i < count; i +=2 ){
        decipher( &p[i]); // byte swapping is done inside trice tool according to endianness.
    }
}
#endif // #ifdef TRICE_DECRYPT

//! TriceEncrypt converts to xtea cipher.
//! \param p pointer to 8 byte buffer
void TriceEncrypt( uint32_t* p, unsigned count ){
    for( int i = 0; i < count; i +=2 ){
        encipher( &p[i] ); // byte triceSwap is done inside trice tool
    }
}

#endif // #ifdef TRICE_ENCRYPT

#if (TRICE_PACKAGE_FRAMING == TRICE_COBS_FRAMING) || (TRICE_ENCODING == TRICE_LEGACY_ENCODING)

// todo: This code is not identical to https://github.com/rokath/cobs/blob/master/cobs.c

//! TriceCOBSEncode stuffs "length" bytes of data at the location pointed to by "input"
//! and writes the output to the location pointed to by "output".
//! Returns the number of bytes written to "output".
//! Remove the "restrict" qualifiers if compiling with a pre-C99 C dialect.
//! (copied and adapted from https://github.com/jacquesf/COBS-Consistent-Overhead-Byte-Stuffing/blob/master/cobs.c)
unsigned TriceCOBSEncode( uint8_t* restrict output, const uint8_t * restrict input, unsigned length){
    unsigned read_index = 0;
    unsigned write_index = 1;
    unsigned code_index = 0;
    uint8_t code = 1;
    while(read_index < length)
    {
        if(input[read_index] == 0)
        {
            output[code_index] = code;
            code = 1;
            code_index = write_index++;
            read_index++;
        }
        else
        {
            output[write_index++] = input[read_index++];
            code++;
            if(code == 0xFF)
            {
                output[code_index] = code;
                code = 1;
                code_index = write_index++;
            }
        }
    }
    output[code_index] = code;
    return write_index;
}

#endif // #if PACKAGE_FRAMING == COBS_FRAMING

#if TRICE_PACKAGE_FRAMING == TRICE_TCOBSV1_FRAMING && (TRICE_ENCODING == TRICE_TREX_ENCODING)

// This code is a copy of https://github.com/rokath/tcobs/blob/master/TCOBSv1/tcobsInternal.h.
//

#define N  0xA0 //!< sigil byte 0x101ooooo, offset 0-31
#define Z1 0x20 //!< sigil byte 0x001ooooo, offset 0-31
#define Z2 0x40 //!< sigil byte 0x010ooooo, offset 0-31
#define Z3 0x60 //!< sigil byte 0x011ooooo, offset 0-31
#define F2 0xC0 //!< sigil byte 0x110ooooo, offset 0-31
#define F3 0xE0 //!< sigil byte 0x111ooooo, offset 0-31
#define F4 0x80 //!< sigil byte 0x100ooooo, offset 0-31
#define R2 0x08 //!< sigil byte 0x00001ooo, offset 0-7
#define R3 0x10 //!< sigil byte 0x00010ooo, offset 0-7
#define R4 0x18 //!< sigil byte 0x00011ooo, offset 0-7

// This code is a copy of https://github.com/rokath/tcobs/blob/master/TCOBSv1/tcobsEncode.c
//

//! ASSERT checks for a true condition, otherwise stop.
//! This macro was used during development to verify the code.
#define ASSERT( condition ) //  do{ if( !(condition) ){ for(;;){} } }while(0);

//! OUTB writes a non-sigil byte to output and increments offset. 
//! If offset reaches 31, a NOP sigil byte is inserted and offset is then set to 0.
#define OUTB( by ) do{ \
    *o++ = by; \
    offset++; \
    ASSERT( offset <= 31 ); \
    if( offset == 31 ){ \
        *o++ = N | 31; \
        offset = 0; \
    } \
    } while( 0 ); 

//! OUT_zeroSigil writes one of the sigil bytes Z1, Z3, Z3 
//! according to zeroCount and sets zeroCount=0 and offset=0.
#define OUT_zeroSigil do{ \
    ASSERT( b_1 == 0  ); \
    ASSERT( (fullCount|reptCount) == 0 ); \
    ASSERT( 1 <= zeroCount && zeroCount <= 3 ); \
    ASSERT( offset <= 31 ); \
    *o++ = (zeroCount << 5) | offset; \
    offset = 0; \
    zeroCount = 0; \
    }while(0);

//! OUT_fullSigil writes one of the sigil bytes F2, F3, F4 
//! according to fullCount and sets fullCount=0 and offset=0.
#define OUT_fullSigil do{ \
    ASSERT( b_1 == 0xFF ); \
    ASSERT( (zeroCount|reptCount) == 0 ); \
    ASSERT( 2 <= fullCount && fullCount <= 4 ); \
    ASSERT( offset <= 31 ); \
    *o++ = 0x80 | (fullCount << 5) | offset; \
    offset = 0; \
    fullCount = 0; \
    }while(0);

//! OUT_reptSigil writes one of the sigil bytes R2, R3, R4 
//! according to reptCount and sets reptCount=0 and offset=0.
//! If offset is bigger than 7 a NOP sigil byte is inserted.
#define OUT_reptSigil do{ \
    ASSERT( (zeroCount|fullCount) == 0 ); \
    ASSERT( 2 <= reptCount && reptCount <= 4 ); \
    ASSERT( offset <= 31 ); \
    if( offset > 7 ){ \
        *o++ = N | offset; \
        offset = 0; \
    } \
    *o++ = ((reptCount-1) << 3) | offset; \
    offset = 0; \
    reptCount = 0; \
    }while(0);

int TCOBSEncode( void * restrict output, const void * restrict input, size_t length){
    uint8_t* o = output; // write pointer
    uint8_t* out = output;
    uint8_t const * i = input; // read pointer
    uint8_t const * limit = (uint8_t*)input + length; // read limit
    uint8_t zeroCount = 0; // counts zero bytes 1-3 for Z1-Z3
    uint8_t fullCount = 0; // counts 0xFF bytes 1-4 for FF and F2-F4
    uint8_t reptCount = 0; // counts repeat bytes 1-4 for !00 and R2-R4,
    uint8_t b_1 = 0; // previous byte
    uint8_t b = 0; // current byte
    uint8_t offset = 0; // link to next sigil or buffer start looking backwards
    // comment syntax:
    //     Sigil bytes chaining is done with offset and not shown explicitly.
    //     All left from comma is already written to o and if, only partially shown.
    //     n is 0...3|4 and m is n+1, representing count number.
    //     zn, fn, rn right from comma is count in variables, if not shown, then 0.
    //     At any moment only one of the 3 counters can be different from 0.
    //     Zn, Fn, Rn, Nn are (written) sigil bytes.
    //     Between comma and dot are the 2 values b_1 and b.
    //     3 dots ... means it is unknown if bytes follow. 
    //     !00 is a byte != 00.
    //     !FF is a byte != FF.
    //     aa is not 00 and not FF and all aa in a row are equal.
    //     xx yy and zz are any bytes.
    //     Invalid b_1 and b are displayed as --.

    if( length >= 2 ){ // , -- --. xx yy ...
        b = *i++; // , -- xx. yy ...
        for(;;){ // , zn|fn|rn -- xx. yy ...
            b_1 = b; // , xx --. yy ...
            b = *i++; // , xx yy. ...

            if( limit - i > 0 ){ // , xx yy. zz ...

                // , z0 00 00. -> , z1 -- 00.
                // , z0 00 00. 00 -> , z2 -- 00.

                if( (b_1 | b) == 0){ // , zn 00 00. zz ...      
                    zeroCount++; // , zm -- 00. zz ...
                    if( zeroCount == 2 ){ // , z2 -- 00. zz ...
                        zeroCount = 3; // , z3 -- --. zz ...
                        OUT_zeroSigil // Z3, -- --. zz ...
                        b = *i++; // , -- zz. ...
                        if( limit - i == 0 ){ // , -- zz.
                            goto lastByte;
                        }
                        // , -- xx. yy ...
                    } 
                    continue; // , zn -- xx. yy ...  
                }

                // , f0 FF FF. -> , f1 -- FF.
                // , f0 FF FF. FF -> , f2 -- FF.
                // , f0 FF FF. FF FF -> , f3 -- FF.

                if( (b_1 & b) == 0xFF ){ // , fn FF FF. zz ...
                    fullCount++; // , fm -- FF. zz ...
                    if( fullCount == 3 ){ // , f3 -- FF. zz ...
                        fullCount = 4; // , f4 -- --. zz ...
                        OUT_fullSigil // F4, -- --. zz ...
                        b = *i++; // , -- zz. ...
                        if( limit - i == 0 ){ // , -- zz.
                            goto lastByte;
                        }
                        // , -- xx. yy ...
                    } 
                    continue; // , fn -- xx. yy ...
                }

                // , r0 aa aa. -> , r1 -- aa.
                // , r0 aa aa. aa -> , r2 -- aa.
                // , r0 aa aa. aa aa -> , r3 -- aa.
                // , r0 aa aa. aa aa aa -> , r4 -- aa.
                
                if( b_1 == b  ){ // , rn aa aa. xx ...
                    ASSERT( b_1 != 0 );
                    ASSERT( b_1 != 0xFF );
                    reptCount++; // , rm -- aa. xx ... 
                    if( reptCount == 4 ){ // , r4 -- aa. xx ...
                        OUTB( b ) // aa, r4 -- --. xx ...
                        OUT_reptSigil // aa R4, -- --. xx ...
                        b = *i++; // , -- xx. ...
                        if( limit - i == 0 ){ // , -- xx.
                            goto lastByte;
                        }
                        // , -- xx. yy ...
                    } 
                    continue; // , r1|r2|r3 -- aa. yy ... 
                }       // OR // , -- xx. yy ...

                // , zn|fn|rn xx yy. zz ... (at this point is b_1 != b)

                // handle counts
                if( zeroCount ) { // , z1|z2 00 aa. xx ...
                    ASSERT( 1 <= zeroCount && zeroCount <= 2 )
                    ASSERT( b_1 == 0 )
                    ASSERT( b_1 != b )
                    zeroCount++; // , z2|z3 -- aa. xx ...
                    OUT_zeroSigil // Z2|Z3, -- aa. xx ...
                    continue; 
                }
                if( fullCount ) { // , f1|f2|f3 FF !FF. xx ...
                    ASSERT( 1 <= fullCount && fullCount <= 3 )
                    ASSERT( b_1 == 0xFF )
                    ASSERT( b_1 != b )
                    fullCount++; // , f2|f3|f4 -- !FF. xx ...
                    OUT_fullSigil // Fn, -- !FF. xx ...
                    continue;
                }
                if( reptCount ) { // , r1|r2|r3 aa !aa. xx ...
                    ASSERT( 1 <= reptCount && reptCount <= 3 )
                    ASSERT( b_1 != 0 )
                    ASSERT( b_1 != 0xFF )
                    ASSERT( b_1 != b )
                    if( reptCount == 1 ){ // , r1 aa !aa. xx ...
                        reptCount = 0; // clear
                        OUTB( b_1 ) // aa, r0 aa !aa. xx ...
                        OUTB( b_1 ) // aa aa, -- !aa. xx ...
                        continue;
                    }
                    *o++ = b_1; // aa, r2|r3 -- !aa. xx ...
                    offset++;
                    OUT_reptSigil // aa R1|R2|R3, -- !aa. xx ...
                    continue;
                }

                // at this point all counts are 0, b_1 != b and b_1 = xx, b == yy
                ASSERT( zeroCount == 0 )
                ASSERT( fullCount == 0 )
                ASSERT( reptCount == 0 )
                ASSERT( b_1 != b )

                // , xx yy. zz ...
                if( b_1 == 0 ){ // , 00 !00. xx ...
                    ASSERT( b != 0 )
                    zeroCount++; // , z1 -- !00. xx ...
                    OUT_zeroSigil
                    continue;  // Z1, -- !00. xx ...
                }
                if( b_1 == 0xFF ){ // , FF !FF. xx ...
                    ASSERT( b != 0xFF )
                    OUTB( 0xFF ); // FF, -- !FF. xx ...
                    continue; 
                }
                
                // , aa xx. yy ...
                ASSERT( 1 <= b_1 && b_1 <= 0xFE )
                OUTB( b_1 ) // aa, -- xx. yy ...
                continue; 

            }else{ // last 2 bytes
                // , zn|fn|rn xx yy.
                if( (zeroCount | fullCount | reptCount) == 0){ // , xx yy.
                    if( b_1 == 0 && b == 0 ){ // , 00 00.
                        *o++ = Z2 | offset; // Z2, -- --.
                        return o - out;
                    }
                    if( b_1 == 0xFF && b == 0xFF ){ // , FF FF.
                        *o++ = F2 | offset; // F2, -- --.
                        return o - out;
                    }
                    if( b_1 == 0 ){ // , 00 xx.
                        zeroCount = 1; // , z1 -- xx.
                        OUT_zeroSigil // Z1, -- xx.
                        goto lastByte;
                    }
                    // , aa xx.
                    ASSERT( b_1 != 0 )
                    OUTB( b_1 ) // aa, -- xx.
                    goto lastByte;
                }

                // At this point exactly one count was incremented.
                // If a count is >0 it is necessarily related to b_1.
                
                if( zeroCount == 1 ) { // , z1 00 yy
                    ASSERT( fullCount == 0 )
                    ASSERT( reptCount == 0 )
                    ASSERT( b_1 == 0 )
                    if( b != 0 ){ // , z1 00 !00.
                        zeroCount++; // , z2 -- !00.
                        OUT_zeroSigil // Z2, -- !00.
                        goto lastByte;
                    }
                    if( b == 0 ){ // , z1 00 00.
                        *o++ = Z3 | offset; // Z3, -- --.
                        return o - out;
                    }
                    ASSERT( 0 )
                }

                if( zeroCount == 2 ) { // , z2 00 !00.
                    ASSERT( fullCount == 0 )
                    ASSERT( reptCount == 0 )
                    ASSERT( b_1 == 0 )
                    ASSERT( b != 0 )
                    zeroCount = 3; // , z3 -- aa.
                    OUT_zeroSigil // Z3, -- aa.
                    goto lastByte;
                }

                if( fullCount == 1 ) { // , f1 FF yy.
                    ASSERT( zeroCount == 0 )
                    ASSERT( reptCount == 0 )
                    ASSERT( b_1 == 0xFF )
                    if( b == 0xFF ){ // , f1 FF FF.
                        ASSERT( offset <= 31 );
                        *o++ = F3 | offset; // F3, -- --.
                        return o - out;
                    }
                    fullCount = 2; // , f2 -- yy.
                    OUT_fullSigil  // F2, -- yy.
                    goto lastByte;
                }

                if( fullCount == 2 ) { // , f2 FF yy.
                    ASSERT( zeroCount == 0 )
                    ASSERT( reptCount == 0 )
                    ASSERT( b_1 == 0xFF )
                    if( b == 0xFF ){ // , f2 FF FF.
                        ASSERT( offset <= 31 );
                        *o++ = F4 | offset; // F4, -- --.
                        return o - out;
                    }
                    // , f2 FF !FF
                    fullCount++; // , f3 -- !FF.
                    OUT_fullSigil  // F3, -- !FF.
                    goto lastByte;
                }

                if( fullCount == 3 ) { // , f3 FF yy.
                    ASSERT( zeroCount == 0 )
                    ASSERT( reptCount == 0 )
                    ASSERT( b_1 == 0xFF )
                    if( b == 0xFF ){ // , f3 FF FF.
                        OUT_fullSigil  // F3, FF FF.
                        ASSERT( offset <= 31 );
                        *o++ = F2 | offset; // F3 F2, -- --.
                        return o - out; // option: F4 FF, -- --. is also right
                    }
                    // , f3 FF !FF.
                    fullCount = 4; // , f4 -- xx.
                    OUT_fullSigil  // F4, -- xx.
                    goto lastByte;
                }

                if( reptCount == 1 ) { // , r1 aa yy. 
                    ASSERT( zeroCount == 0 )
                    ASSERT( fullCount == 0 )
                    ASSERT( b_1 != 0 )
                    ASSERT( b_1 != 0xFF )
                    if( b_1 == b ){ // , r1 aa aa.
                        OUTB( b_1 ) // aa, r1 -- aa.
                        ASSERT( offset <= 31 );
                        if( offset > 7 ){ 
                            *o++ = N | offset;
                            offset = 0;
                        }
                        *o++ = R2 | offset; // aa R2, -- --.
                        return o - out;
                    }
                    OUTB( b_1 ) // aa, r0 aa -- yy.
                    OUTB( b_1 ) // aa aa, -- yy.
                    goto lastByte;
                }
                if( reptCount == 2 ) { // , r2 aa yy.
                    ASSERT( zeroCount == 0 )
                    ASSERT( fullCount == 0 )
                    ASSERT( b_1 != 0 )
                    ASSERT( b_1 != 0xFF )
                    if( b_1 == b ){ // , r2 aa aa.
                        OUTB( b_1 ) // aa, r2 -- aa.
                        ASSERT( offset <= 31 );
                        if( offset > 7 ){ 
                            *o++ = N | offset;
                            offset = 0;
                        }
                        *o++ = R3 | offset; // aa R3, -- --.
                        return o - out;
                    }                    
                    OUTB( b_1 ) // aa, r2 -- yy.
                    OUT_reptSigil  // aa R2, -- xx.
                    goto lastByte;
                }
                if( reptCount == 3 ) { // , r3 aa yy.
                    ASSERT( zeroCount == 0 )
                    ASSERT( fullCount == 0 )
                    ASSERT( b_1 != 0 )
                    ASSERT( b_1 != 0xFF )
                    OUTB( b_1 ) // aa, r3 -- yy. 
                    if( b_1 == b ){ // , r3 aa aa.
                        ASSERT( offset <= 31 );
                        if( offset > 7 ){
                            *o++ = N | offset;
                            offset = 0;
                        }
                        *o++ = R4 | offset; // aa R4, -- --.
                        return o - out;
                    }
                    // aa, r3 -- yy.
                    OUT_reptSigil  // aa R3, -- xx.
                    goto lastByte;
                }

                ASSERT( 0 ) // will not be reached 
            }
        }
    }
    if( length == 0 ){ // , -- --.
        return 0;
    }
    if( length == 1 ){ // , . xx
        b = *i; // , -- xx.
        goto lastByte;
    }

lastByte: // , -- xx.
    if( b == 0 ){ // , -- 00.
        ASSERT( offset <= 31 );
        *o++ = Z1 | offset; // Z1, -- --.
        return o - out;
    }else{ // , -- aa.
        *o++ = b; // aa|ff, -- --.
        offset++;
        ASSERT( offset <= 31 );
        *o++ = N | offset; // aa Nn, -- --.
        return o - out;
    }
}

#endif // #if TRICE_PACKAGE_FRAMING == TRICE_TCOBSV1_FRAMING

#if TRICE_PACKAGE_FRAMING == TRICE_TCOBSV2_FRAMING && (TRICE_ENCODING == TRICE_TREX_ENCODING)

// This code is a copy of https://github.com/rokath/tcobs/blob/master/TCOBSv2/tcobsInternal.h

#define N  0x00 //!< N  is the sigil byte 0x101ooooo, without operation with offsets 1-31 to keep the sigil chain linked.
//         0x10 is inside N
#define Z0 0x20 //!< Z0 is the sigil byte 0x001ooooo, a CCQN zeroCount cipher 0 with offsets 0-31.
//         0x30 is inside Z0
#define R1 0x40 //!< R1 is the sigil byte 0x0100oooo, a CCTN repeatCount cipher 1 with offsets 0-15.
#define Z2 0x50 //!< Z2 is the sigil byte 0x0101oooo, a CCQN zeroCount cipher 2 with offsets 0-15.
#define Z1 0x60 //!< Z1 is the sigil byte 0x011ooooo, a CCQN zeroCount cipher 1 with offsets 0-31.
//         0x70 is inside Z1
#define R0 0x80 //!< R0 is the sigil byte 0x100ooooo, a CCTN repeatCount cipher 0 with offsets 0-31.
//         0x90 is inside R0
#define R2 0xA0 //!< R2 is the sigil byte 0x1010oooo, a CCTN repeatCount cipher 2 with offsets 0-15.
#define Z3 0xB0 //!< Z3 is the sigil byte 0x0001oooo, a CCQN zeroCount cipher 3 with offsets 0-15.
#define F1 0xC0 //!< F1 is the sigil byte 0x110ooooo, a CCQN fullCount cipher 1 with offsets 0-31.
//         0xD0 is inside F1
#define F2 0xE0 //!< F2 is the sigil byte 0x1110oooo, a CCQN fullCount cipher 2 with offsets 0-15.
#define F3 0xF0 //!< F3 is the sigil byte 0x1111oooo, a CCQN fullCount cipher 3 with offsets 0-14.
#define F0 0xFF //!< F0 is the sigil byte 0x11111111, a CCQN fullCount cipher 0 with offset 0.

// https://github.com/rokath/tcobs/blob/master/TCOBSv2/tcobsEncode.c

//! ASSERT checks for a true condition, otherwise stop.
//! For the encoding this macro was used just to verify the code during development.
#define ASSERT( condition ) // do{ if( !(condition) ){ return -__LINE__; } }while(0);

//! MAX_CIPHERS is max expected sigil bytes of one kind in a row. 
//! 3^24 = 282.429.536.481, we do not expect so much equal bytes.
#define MAX_CIPHERS 24 

static int writeZeroCount( uint8_t ** out, int * count, int * distance );
static int writeFullCount( uint8_t ** out, int * count, int * distance );
static int writeRepeatCount( uint8_t ** out, uint8_t aa, int * count, int * distance );

static int writeNoopSigil( uint8_t ** out, int * distance);
static int writeLastSigil( uint8_t ** out, int * distance);

static int CCQNtoCCQNZ( uint8_t* ciphers, int count );
static int CCQNtoCCQNF( uint8_t* ciphers, int count );
static int CCTNtoCCTNR( uint8_t* ciphers, int count );

static int ntoq( int num, uint8_t* buf );
static int ntot( int num, uint8_t* buf );

static int ntoCCQNZ( int num, uint8_t* buf );
static int ntoCCQNF( int num, uint8_t* buf );
static int ntoCCTNR( int num, uint8_t* buf );

static int ntoCCQNZgeneric( int num, uint8_t* buf );
static int ntoCCQNFgeneric( int num, uint8_t* buf );
static int ntoCCTNRgeneric( int num, uint8_t* buf );

int TCOBSEncode( void * restrict output, const void * restrict input, size_t length){
    uint8_t const * i = input; // read pointer
    uint8_t const * limit = (uint8_t*)input + length; // read limit
    uint8_t b_1; // previous byte
    uint8_t b; // current byte
    int err = 0;
    uint8_t * out = output; //!< out is the output write pointer.
    int distance = 0; //<! distance is the byte count to the next sigil in sigil chain.
    int zeroCount = 0; //!< zeroCount is the number of accumulated 00-bytes.
    int fullCount = 0; //!< fullCount is the number of accumulated FF-bytes.
    int reptCount = 0; //!< reptCount is the number of accumulated equal bytes.

    // comment syntax:
    //     Sigil bytes chaining is done with offset and not shown explicitly.
    //     All left from comma is already written to o and if, only partially shown.
    //     n is 0..., representing count number.
    //     zn, fn, rn right from comma is count in variables, if not shown, then 0.
    //     At any moment only one of the 3 counters can be different from 0.
    //     Zn, Fn, Rn, Nn are (written) sigil bytes.
    //     Between comma and dot are the 2 values b_1 and b.
    //     3 dots ... means it is unknown if bytes follow. 
    //     !00 is a byte != 00.
    //     !FF is a byte != FF.
    //     aa is not 00 and not FF and all aa in a row are equal.
    //     xx yy and zz are any bytes.
    //     Invalid b_1 and b are displayed as --.

    if( length >= 2 ){ // , -- --. xx yy ... (at least 2 bytes)
        b = *i++; // , -- xx, yy ...
        for(;;){ // , zn|fn|rn -- xx. yy ... (possibly one of the 3 counters could be > 0 here, but only one)
            if( err ){
                return err;
            }
            b_1 = b; // , xx --. yy ...
            b = *i++; // , xx yy. ...

            if( i < limit ){ // , zn|fn|rn xx yy. zz ... (at least one more byte)  
                if( b_1 == b ){ // , zn|fn|rn xx xx. zz ... 
                    if( b == 0 ){ // , zn 00 00. zz ... 
                        ASSERT( (fullCount|reptCount) == 0 );
                        zeroCount++; // , zn -- 00. zz ... 
                        continue; 
                    }
                    if( b == 0xFF ){
                        ASSERT( (zeroCount|reptCount) == 0 );
                        fullCount++; // , fn -- FF. zz ... 
                        continue; 
                    }
                    //( b == b_1 )
                    ASSERT( (fullCount|zeroCount) == 0 );
                    reptCount++; // , rn -- AA. zz ... 
                    continue; 
                }else{ // , zn|fn|rn xx yy. zz ... (xx != yy)
                    if( b_1 == 0 ) { // , zn 00 yy. zz ...
                        ASSERT( (fullCount|reptCount) == 0 );
                        zeroCount++; // , zn -- yy. zz ...
                        err = writeZeroCount(&out, &zeroCount, &distance); // , -- yy. zz ...
                        continue;
                    }
                    if( b_1 == 0xFF ) { // , fn FF yy. zz ...
                        ASSERT( (zeroCount|reptCount) == 0 );
                        fullCount++; // , fn -- yy. zz ...
                        err = writeFullCount(&out, &fullCount, &distance); // , -- yy. zz ...
                        continue;
                    }
                    // , rn AA yy. zz ...
                    if( distance == 31 ){
                        err = writeNoopSigil( &out, &distance);
                        if( err ){
                            return err;
                        }
                    }
                    *out++ = b_1; // AA, rn -- yy. zz ...
                    distance++;
                    if( reptCount ){ // AA, rn -- yy. zz ... (n>=1)
                        ASSERT( (fullCount|zeroCount) == 0 );
                        err = writeRepeatCount(&out, b_1, &reptCount, &distance); // AA, -- yy. zz ...
                    }
                    continue;
                }
            }else{ // last 2 bytes
                // , zn|fn|rn xx yy.
                if( b_1 == b ){ // , zn|fn|rn xx xx. 
                    if( b == 0 ){ // , zn 00 00. 
                        ASSERT( (fullCount|reptCount) == 0 ); 
                        zeroCount += 2; // , zn -- --. 
                        err = writeZeroCount(&out, &zeroCount, &distance);
                        if( err ){
                            return err;
                        }
                        return out - (uint8_t*)output;
                    }
                    if( b == 0xFF ){ // , zn FF FF. 
                        ASSERT( (zeroCount|reptCount) == 0 );
                        fullCount += 2; // , fn -- --. 
                        err = writeFullCount(&out, &fullCount, &distance);
                        if( err ){
                            return err;
                        }
                        return out - (uint8_t*)output;
                    }
                    // , zn AA AA.
                    ASSERT( (fullCount|zeroCount) == 0 );
                    reptCount +=1; // , rn -- AA.
                    if( distance == 31 ){
                        err = writeNoopSigil( &out, &distance);
                        if( err ){
                            return err;
                        }
                    }
                    *out++ = b; // AA, rn -- --.
                    distance++;
                    err = writeRepeatCount(&out, b, &reptCount, &distance);
                    if( err ){
                        return err;
                    }
                    if( distance > 0 ){
                        err = writeNoopSigil( &out, &distance);
                        if( err ){
                            return err;
                        }
                    }
                    return out - (uint8_t*)output;
                }else{ // , zn|fn|rn xx yy. (xx != yy)
                    if( b_1 == 0 ) { // , zn 00 yy.
                        ASSERT( (fullCount|reptCount) == 0 );
                        zeroCount++; // , zn -- yy.
                        err = writeZeroCount(&out, &zeroCount, &distance); // , -- yy.
                        goto lastByte;
                    }
                    if( b_1 == 0xFF ) { // , fn FF yy.
                        ASSERT( (zeroCount|reptCount) == 0 );
                        err = fullCount++; // , fn -- yy.
                        err = writeFullCount(&out, &fullCount, &distance); // , -- yy.
                        goto lastByte;
                    }
                    // , rn AA yy.
                    if( distance == 31 ){
                        err = writeNoopSigil( &out, &distance);
                        if( err ){
                            return err;
                        }
                    }
                    *out++ = b_1; // AA, rn -- yy.
                    distance++;
                    if( reptCount ){ // AA, rn -- yy. (n>=1)
                        ASSERT( (fullCount|zeroCount) == 0 );
                        err = writeRepeatCount(&out, b_1, &reptCount, &distance); // AA, -- yy.
                    }
                    goto lastByte;
                }
            }
        }
    }
    if( length == 0 ){ // , -- --.
        return 0;
    }
    if( length == 1 ){ // , . xx
        b = *i; // , -- xx.
        // (no need) goto lastByte;
    }
lastByte: // , -- xx.
    if( err ){
        return err;
    }
    if( b == 0 ){ // , -- 00.
        zeroCount++; // , zn -- --. (n=1)
        err = writeZeroCount(&out, &zeroCount, &distance);
        if( err ){
            return err;
        }
        // (no need) writeLastSigil(&out, &distance);
        return out - (uint8_t*)output;
    }
    if( b == 0xFF ){ // , -- FF.
        fullCount++; // , fn -- --. (n=1)
        err = writeFullCount(&out, &fullCount, &distance); // could be F0
        if( err ){
            return err;
        }
        if( distance > 1 ){ // a single F0 at the end needs a terminating N sigil
            err = writeLastSigil(&out, &distance); // if preceeded by a non sigil byte
            if( err ){
                return err;
            }
        }
        return out - (uint8_t*)output;
    }
    if( distance == 31 ){
        err = writeNoopSigil( &out, &distance);
        if( err ){
            return err;
        }
    }
    *out++ = b;
    distance++;
    err = writeLastSigil(&out, &distance);
    if( err ){
        return err;
    }
    return out - (uint8_t*)output;
}

//! writeNoopSigil inserts a N-sigil carrying the *distance and sets *distance to 0.
static int writeNoopSigil( uint8_t ** out, int * distance){
    ASSERT( 0 < *distance && *distance < 32 );
    **out = N | *distance;
    *out += 1;
    *distance = 0;
    return 0;
}

//! writeLastSigil terminates encoded buffer by appending a sigil byte, if *distance is not 0.
static int writeLastSigil( uint8_t ** out, int * distance){
    if( *distance ){
        int err = writeNoopSigil( out, distance);
        if( err < 0 ){
            return err;
        }
    }
    return 0;
}

//! writeZeroCount writes *num 00-bytes encoded as Z-sigil sequence including *distance value.
static int writeZeroCount( uint8_t ** out, int * num, int * distance ){
    uint8_t ciphers[MAX_CIPHERS];
    int ciphersCount = ntoCCQNZ( *num, ciphers );
    int err = 0;
    if( ciphersCount < 0 ){
        return ciphersCount; // err
    }
    ASSERT( ciphersCount != 0 )
    for( int i = 0; i < ciphersCount; i++ ){
        if( err < 0 ){
            return err;
        }
        switch( ciphers[i] ){
            case Z0:
                ASSERT( *distance <= 31 );
                **out = Z0 | *distance;
                *out += 1;
                *distance = 0;
                continue;
            case Z1:
                ASSERT( *distance <= 31 );
                **out = Z1 | *distance;
                *out += 1;
                *distance = 0;
                continue;
            case Z2:
                if( *distance > 15 ){
                    err = writeNoopSigil( out, distance);
                }
                **out = Z2 | *distance;
                *out += 1;
                *distance = 0;
                continue;
            case Z3:
                if( *distance > 15 ){
                    err = writeNoopSigil( out, distance);
                }
                **out = Z3 | *distance;
                *out += 1;
                *distance = 0;
                continue;
            default: ASSERT( 0 );
        }
    }
    *num = 0;
    return 0;
}

//! writeFullCount writes *num FF-bytes encoded as F-sigil sequence including *distance value.
static int writeFullCount( uint8_t ** out, int * num, int * distance ){
    uint8_t ciphers[MAX_CIPHERS];
    int ciphersCount = ntoCCQNF( *num, ciphers );
    int err = 0;
    if( ciphersCount < 0 ){
        return ciphersCount; // err
    }
    ASSERT( ciphersCount != 0 )
    for( int i = 0; i < ciphersCount; i++ ){
        if( err < 0 ){
            return err;
        }
        switch( ciphers[i] ){
            case F0:
                if( ciphersCount == 1 ){ // a single F0 (==FF) can be treated as orinary byte
                    if( *distance == 31 ){
                        err = writeNoopSigil( out, distance);
                    }
                    **out = F0;
                    *out += 1;
                    *distance += 1;
                }else if( ciphersCount > 1 ){
                    if( i == 0 ){ // a first F0 cannot carry a distance
                        if( *distance > 0 ){
                            err = writeNoopSigil( out, distance);
                        }
                    }
                    **out = F0;
                    *out += 1;
                }
                continue;
            case F1:
                ASSERT( *distance <= 31 );
                **out = F1 | *distance;
                *out += 1;
                *distance = 0;
                continue;
            case F2:
                if( *distance > 15 ){
                     err = writeNoopSigil( out, distance);
                }
                **out = F2 | *distance;
                *out += 1;
                *distance = 0;
                continue;
            case F3:
                if( *distance > 14 ){
                     err = writeNoopSigil( out, distance);
                }
                **out = F3 | *distance;
                *out += 1;
                *distance = 0;
                continue;
            default: ASSERT( 0 );
        }
    }
    *num = 0;
    return 0;
}

//! writeRepeatCount writes *num aa-bytes encoded as aa followed by an R-sigil sequence including *distance value.
static int writeRepeatCount( uint8_t ** out, uint8_t aa, int * num, int * distance ){
    int err = 0;
    if( *num == 1 ){
        if( *distance == 31 ){
            err = writeNoopSigil( out, distance);
            if( err ){
                return err;
            }
        }
        **out = aa;
        *out += 1;
        *distance += 1;
    }else{
        uint8_t ciphers[MAX_CIPHERS];
        int ciphersCount = ntoCCTNR( *num, ciphers );

        if( ciphersCount < 0 ){
            return ciphersCount; // err
        }
        ASSERT( ciphersCount != 0 )
        for( int i = 0; i < ciphersCount; i++ ){
            if( err ){
                return err;
            }
            switch( ciphers[i] ){
                case R0:
                    ASSERT( *distance <= 31 );
                    **out = R0 | *distance;
                    *out += 1;
                    *distance = 0;
                    continue;
                case R1:
                    if( *distance > 15 ){
                        err = writeNoopSigil( out, distance);
                    }
                    **out = R1 | *distance;
                    *out += 1;
                    *distance = 0;
                    continue;
                case R2:
                    if( *distance > 15 ){
                        err = writeNoopSigil( out, distance);
                    }
                    **out = R2 | *distance ;
                    *out += 1;
                    *distance = 0;
                    continue;
                default: ASSERT( 0 );
            }
        }
    }
    *num = 0;
    return 0;
}

//! CCQNgenericStartValue computes CCQN generic start value of n and returns it.
//! It also fills ciphersCount with the appropriate number of ciphers.
//! n->ciphersCount: 0->0, 1->1, 4->1, 5->2
//! n->genericStart: 0->0, 1->1, 4->1, 5->5 (5=4^0+4^)
static int CCQNgenericStartValue( int num, int * ciphersCount ){
    *ciphersCount = 0;
    int power = 1; // 4^0  // 0->0, 1->1, 4->1, 5->2
    int genericStart = 0;
    while( genericStart + power <= num ){
        genericStart += power; // 4^0 + 4^1 + 4^2 + ...
        power *= 4;
        *ciphersCount += 1;
    }
    return genericStart;
}

//! CCTNgenericStartValue computes CCQN generic start value of n and returns it.
//! It also fills ciphersCount with the appropriate number of ciphers.
//! n->ciphersCount: 0->0, 1->1, 4->1, 5->2
//! n->genericStart: 0->0, 1->1, 4->1, 5->5 (5=4^0+4^)
static int CCTNgenericStartValue( int num, int * ciphersCount ){
    *ciphersCount = 0;
    int power = 1; // 3^0  // 0->0, 1->1, 3->1, 4->2
    int genericStart = 1;
    while( genericStart + power <= num ){
        genericStart += power; // 1 + 3^0 + 3^1 + 3^2 + ...
        power *= 3;
        *ciphersCount += 1;
    }
    return genericStart;
}

//! ntoCCQNZ converts num into a CCQN cipher sequence coded as Z sigils to ciphers and returns count of ciphers.
static int ntoCCQNZgeneric( int num, uint8_t* ciphers ){
    ASSERT( num > 0 ){
        int ciphersCount = 0;
        int gsv = CCQNgenericStartValue( num, &ciphersCount );
        int n = num - gsv;
        int qcount = ntoq( n, ciphers );
        memmove(ciphers + ciphersCount - qcount, ciphers, qcount);
        memset(ciphers, 0, ciphersCount - qcount );
        return CCQNtoCCQNZ( ciphers, ciphersCount );
    }
}

//! ntoCCQNF converts num into a CCQN cipher sequence coded as F sigils to ciphers and returns count of ciphers.
static int ntoCCQNFgeneric( int num, uint8_t* ciphers ){
    ASSERT( num > 0 ){
        int ciphersCount = 0;
        int gsv = CCQNgenericStartValue( num, &ciphersCount );
        int n = num - gsv;
        int qcount = ntoq( n, ciphers );
        memmove(ciphers + ciphersCount - qcount, ciphers, qcount);
        memset(ciphers, 0, ciphersCount - qcount );
        return CCQNtoCCQNF( ciphers, ciphersCount );
    }
}

//! ntoCCQNR converts num into a CCQN cipher sequence coded as F sigils to ciphers and returns count of ciphers.
static int ntoCCTNRgeneric( int num, uint8_t* ciphers ){
    ASSERT( num > 0 ){
        int ciphersCount = 0;
        int gsv = CCTNgenericStartValue( num, &ciphersCount );
        int n = num - gsv;
        int tcount = ntot( n, ciphers );
        uint8_t* dest = ciphers + ciphersCount - tcount;
        memmove(dest, ciphers, tcount);
        memset(ciphers, 0, ciphersCount - tcount );
        return CCTNtoCCTNR( ciphers, ciphersCount );
    }
}

//! CCQNtoCCQNZ converts ciphers in ciphers to Z-sigils.
static int CCQNtoCCQNZ( uint8_t* ciphers, int count ){
    for(int i = 0; i < count; i++ ){
        if( ciphers[i] == 0 ){
            ciphers[i] = Z0;
        }else if( ciphers[i] == 1 ){
            ciphers[i] = Z1;
        }else if( ciphers[i] == 2 ){
            ciphers[i] = Z2;
        }else if( ciphers[i] == 3 ){
            ciphers[i] = Z3;
        }else{
            return - __LINE__;
        }
    }
    return count;
}

//! CCQNtoCCQNF converts ciphers in ciphers to F-sigils.
static int CCQNtoCCQNF( uint8_t* ciphers, int count ){
    for(int i = 0; i < count; i++ ){
        if( ciphers[i] == 0 ){
            ciphers[i] = F0;
        }else if( ciphers[i] == 1 ){
            ciphers[i] = F1;
        }else if( ciphers[i] == 2 ){
            ciphers[i] = F2;
        }else if( ciphers[i] == 3 ){
            ciphers[i] = F3;
        }else{
            return - __LINE__;
        }
    }
    return count;
}

//! CCTNtoCCTNR converts ciphers in ciphers to Z-sigils.
static int CCTNtoCCTNR( uint8_t* ciphers, int count ){
    for(int i = 0; i < count; i++ ){
        if( ciphers[i] == 0 ){
            ciphers[i] = R0;
        }else if( ciphers[i] == 1 ){
            ciphers[i] = R1;
        }else if( ciphers[i] == 2 ){
            ciphers[i] = R2;
        }else{
            return - __LINE__;
        }
    }
    return count;
}

//! ntoq converts n into a quaternary cipher sequence to buf and returns count of ciphers.
//! It assumes at least MAX_CIPHERS bytes space in buffer and uses them as scratchpad.
static int ntoq( int n, uint8_t* buffer ){
    int i = MAX_CIPHERS;
    int count;
    while (n){
        int r = n & 3;  // n % 4;
        n     = n >> 2; // n / 4;
        buffer[--i] = r;
    }
    count = MAX_CIPHERS-i;
    memmove(buffer, buffer+i, count);
	return count;
}

//! ntot converts n into a ternary cipher sequence to buf and returns count of ciphers.
//! It assumes at least MAX_CIPHERS bytes space in buffer and uses them as scratchpad.
static int ntot(int n, uint8_t* buffer ){
    int i = MAX_CIPHERS;
    int count;
    while (n){
        int r = n % 3; // option: speedup by computing r and n in one step
        n     = n / 3; // but it makes not much sense, when lookup tables are used
        buffer[--i] = r;
    }
    count = MAX_CIPHERS-i;
    memmove(buffer, buffer+i, count);
	return count;
}

//! ntoCCQNZ converts num into a CCQN cipher sequence coded as Z sigils to buf and returns count of ciphers.
//! It assumes at least MAX_CIPHERS bytes space in buffer and uses them as scratchpad.
static int ntoCCQNZ( int num, uint8_t* buf ){
    ASSERT( num > 0 )
    if( num <= 4 ){
        static uint8_t ciphers[4] = {
            Z0, Z1, Z2, Z3 // 1, 2, 3, 4
        };
        *buf = ciphers[num-1];
        return 1;
    }
    if( num <= 20){
        static uint8_t ciphers[16][2] = {
            {Z0, Z0}, {Z0, Z1}, {Z0, Z2}, {Z0, Z3}, //  5,  6,  7,  8,
            {Z1, Z0}, {Z1, Z1}, {Z1, Z2}, {Z1, Z3}, //  9, 10, 11, 12,
            {Z2, Z0}, {Z2, Z1}, {Z2, Z2}, {Z2, Z3}, // 13, 14, 15, 16,
            {Z3, Z0}, {Z3, Z1}, {Z3, Z2}, {Z3, Z3}  // 17, 18, 19, 20,
        };
        buf[0] = ciphers[num-5][0];
        buf[1] = ciphers[num-5][1];
        return 2;        
    }
    return ntoCCQNZgeneric( num, buf );
}

//! ntoCCQNF converts num into a CCQN cipher sequence coded as F sigils to buf and returns count of ciphers.
static int ntoCCQNF( int num, uint8_t* buf ){
    ASSERT( num > 0 )
    if( num <= 4 ){
        static uint8_t ciphers[4] = {
            F0, F1, F2, F3 // 1, 2, 3, 4
        };
        *buf = ciphers[num-1];
        return 1;
    }
    if( num <= 20 ){
        static uint8_t ciphers[16][2] = {
            {F0, F0}, {F0, F1}, {F0, F2}, {F0, F3}, //  5,  6,  7,  8,
            {F1, F0}, {F1, F1}, {F1, F2}, {F1, F3}, //  9, 10, 11, 12,
            {F2, F0}, {F2, F1}, {F2, F2}, {F2, F3}, // 13, 14, 15, 16,
            {F3, F0}, {F3, F1}, {F3, F2}, {F3, F3}  // 17, 18, 19, 20,
        };
        buf[0] = ciphers[num-5][0];
        buf[1] = ciphers[num-5][1];
        return 2;        
    }
    return ntoCCQNFgeneric( num, buf );
}

//! ntoCCTNR converts num into a CCTN cipher sequence coded as R sigils to buf and returns count of ciphers.
static int ntoCCTNR( int num, uint8_t* buf ){
    ASSERT( num > 1 )
    if( num <= 4 ){
        static uint8_t ciphers[4] = {
            R0, R1, R2 // 2, 3, 4
        };
        *buf = ciphers[num-2];
        return 1;
    }
    if( num <= 13 ){
        static uint8_t ciphers[9][2] = {
            {R0, R0}, {R0, R1}, {R0, R2}, //  5,  6,  7,
            {R1, R0}, {R1, R1}, {R1, R2}, //  8,  9, 10,
            {R2, R0}, {R2, R1}, {R2, R2}, // 11, 12, 13,
        };
        buf[0] = ciphers[num-5][0];
        buf[1] = ciphers[num-5][1];
        return 2;        
    }
    if( num <= 40 ){
        static uint8_t ciphers[27][3] = {
            {R0, R0, R0}, {R0, R0, R1}, {R0, R0, R2}, //  14 +   0,  1,  2
            {R0, R1, R0}, {R0, R1, R1}, {R0, R1, R2}, //  14 +   3,  4,  5
            {R0, R2, R0}, {R0, R2, R1}, {R0, R2, R2}, //  14 +   6,  7,  8

            {R1, R0, R0}, {R1, R0, R1}, {R1, R0, R2}, //  14 +   9, 10, 11
            {R1, R1, R0}, {R1, R1, R1}, {R1, R1, R2}, //  14 +  12, 13, 14
            {R1, R2, R0}, {R1, R2, R1}, {R1, R2, R2}, //  14 +  15, 16, 17

            {R2, R0, R0}, {R2, R0, R1}, {R2, R0, R2}, //  14 +  18, 19, 20
            {R2, R1, R0}, {R2, R1, R1}, {R2, R1, R2}, //  14 +  21, 22, 23
            {R2, R2, R0}, {R2, R2, R1}, {R2, R2, R2}, //  14 +  24, 25, 26
        };
        buf[0] = ciphers[num-14][0];
        buf[1] = ciphers[num-14][1];
        buf[2] = ciphers[num-14][2];
        return 3;        
    }
    return ntoCCTNRgeneric( num, buf );
}

#endif // #if TRICE_PACKAGE_FRAMING == TRICE_TCOBSV2_FRAMING
