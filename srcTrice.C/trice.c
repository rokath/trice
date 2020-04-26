/*! \file trice.c
\details The format strings are ignored - they go not into the target binary. See trice tool doc for details.
The trices (macros) are dumped as 32bit values into a 32 bit fifo. That is the time critical part. 
\li A basic trice (subtrace) consists always of 4 byte: a 16 bit ID with a 16 bit data value.
\li Trices with more data are split into several 32bit basic trice values with IDs = 0 in front.
\li TRICE0, TRICE8_1, TRICE8_2 and TRICE16_1 have 4 bytes size, all others have 8 - 32 bytes size.
\li For trice transmission each basic trice gets a header of additional 4 bytes.
\li The header contains a startbyte, client and server address and a crc8 exOr checksum.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#include "trice.h"
#ifdef ENCRYPT
#include "xteaCrypto.h" // enable this for encryption
#endif

#if 1 == TRICE_PRINTF_ADAPTER
#include <stdarg.h>
#include <stdio.h> 
#endif // #if 1 == TRICE_PRINTF_ADAPTER

#if NO_CODE == TRICE_CODE
#error
void TxStart( void ){
}
void TxContinue( void ){
}

#else // #if NO_CODE == TRICE_CODE

enum{
    noTx,    // no transmission
    triceTx, // trice packet in transmission
    comTx    // communication packet in transmission
};


//! trice fifo instance, here are the trices buffered. used in TRICE macro expansion
ALIGN4 uint32_t triceFifo[ TRICE_FIFO_SIZE>>2 ] ALIGN4_END;

uint32_t wrIndexTriceFifo = 0; //!< trice fifo write index, used inside macros, so must be visible


static int txState = noTx; //!< txState, needed to not mix the packet data during bytes tx

static uint32_t rdIndexTriceFifo = 0; //!< trice fifo read index

//! get one trice from trice fifo
//! am p address for trice id with 2 byte data
TRICE_INLINE void triceFifoPop( uint32_t* p ){
    *p = triceFifo[rdIndexTriceFifo++];
    rdIndexTriceFifo &= TRICE_FIFO_MASK;
}

//! trice item count inside trice fifo
//! \return count of buffered trices
TRICE_INLINE size_t triceFifoDepth( void ){
    return (wrIndexTriceFifo - rdIndexTriceFifo) & TRICE_FIFO_MASK;
}

//! partial prefilled trice message transmit buffer 
ALIGN4 static triceMsg_t triceMsg ALIGN4_END = {
    { TRICE_START_BYTE,  TRICE_LOCAL_ADDR,  TRICE_DISPL_ADDR, 0 }, // crc8
    { 0, 0 } // 16bit ID, 16bit data
};



static uint8_t       *       pRead = (uint8_t*)(&triceMsg + 1); //!< trice message buffer read pointer (initial set to limit for empty triceMsg buffer)
static uint8_t const * const limit = (uint8_t*)(&triceMsg + 1); //!< trice message buffer limit (points behind triceMsg buffer)

//! get next trice byte for transmission from trice message buffer, no depth check here
//!\retval data byte
TRICE_INLINE uint8_t triceMsgNextByte( void ){
    return *pRead++;
}

static size_t triceMsgBufferDepth( void ){
    size_t count = limit - pRead;
    return count;
}

// pull next trice from fifo and prepare triceMsg buffer
static void prepareNextTriceTransmission(void){
    triceFifoPop( (uint32_t*)(&(triceMsg.ld)) );
    pRead = (uint8_t*)&triceMsg;
    triceMsg.hd.crc8  = TRICE_START_BYTE ^ TRICE_LOCAL_ADDR ^ TRICE_DISPL_ADDR
                         ^ triceMsg.ld.load[0]
                         ^ triceMsg.ld.load[1]
                         ^ triceMsg.ld.load[2]
                         ^ triceMsg.ld.load[3];
    #ifdef ENCRYPT
        triceMsg.hd.start = TRICE_START_BYTE;
        triceMsg.hd.cad  = TRICE_LOCAL_ADDR;
        triceMsg.hd.sad  = TRICE_DISPL_ADDR;
        encrypt( (uint8_t*)&triceMsg );
    #endif
}

//! prepare next trice for transmission
//!\retval 0 no next trice
//!\retval 1 next trice in message buffer
static size_t triceNextMsg( void ){
    if( triceFifoDepth() ){
        prepareNextTriceTransmission();
        return 1;
    }
    return 0;
}

#if 1 == TRICE_PRINTF_ADAPTER

//! trice replacement helper for printf() with %s 
//! use only for dynamic generatd strings
//! \param pFmt formatstring
int tricePrintfAdapter( const char* pFmt, ... ){
    va_list va;
    int done;
    static char buffer[TRICE_PRINTF_ADAPTER_BUFFERSIZE];
    
    va_start (va, pFmt );
    done = vsnprintf( buffer, sizeof(buffer), pFmt, va);
    va_end (va);

    triceString( 0, buffer );
    return done;
}

#endif // #if 1 == TRICE_PRINTF_ADAPTER

#if NONE_RUNTIME != TRICE_STRINGS && LESS_FLASH_AND_SPEED == TRICE_CODE

static void triceSpaces( int spaces ){
    while( spaces-->0 )
    {
        TRICE0( Id(27950), " " );
    }
}

#endif

#if NONE_RUNTIME != TRICE_STRINGS && MORE_FLASH_AND_SPEED == TRICE_CODE

static void triceSpaces( int spaces ){
    while (spaces ){
        switch( spaces ){
            case  0: return;
            case  1: TRICE0( Id(14746), " " ); return;
            case  2: TRICE0( Id(32263), "  " ); return;
            case  3: TRICE0( Id(41033), "   " ); return;
            case  4: TRICE0( Id(  500), "    " ); return;
            case  5: TRICE0( Id(23151), "     " ); return;
            case  6: TRICE0( Id(11628), "      " ); return;
            case  7: TRICE0( Id(40825), "       " ); return;
            case  8: TRICE0( Id(63581), "        " ); return;
            case  9: TRICE0( Id(11347), "         " ); return;
            case 10:
            default: TRICE0( Id(46732), "          " ); 
                spaces -= 10;
            break;
        }
    }
    return;
}

#endif

#if RARE_RUNTIME == TRICE_STRINGS && LESS_FLASH_AND_SPEED == TRICE_CODE

static void triceStringUnbound( const char* s ){
    while( *s )
    {
        TRICE8_1( Id(3), "%c", *s );
        s++;
    }
}

#endif 

#if RARE_RUNTIME == TRICE_STRINGS &&  MORE_FLASH_AND_SPEED == TRICE_CODE

//! for performance no check of strlen( s ) here (internal usage)
static void triceStringUnbound( const char* s ){
    size_t len = strlen( s );
    char c1, c2, c3, c4, c5, c6, c7, c8;
    while( len ){
        switch( len ){
            case  0: return;
            case  1: c1=*s++;
                TRICE8_1( Id(36152), "%c", c1 ); return;
            case  2: c1=*s++; c2=*s++;
                TRICE8_2( Id(49862), "%c%c", c1, c2 ); return;
            case  3: c1=*s++; c2=*s++; c3=*s++;
                TRICE8_3( Id(60898), "%c%c%c", c1, c2, c3 ); return;
            case  4: c1=*s++; c2=*s++; c3=*s++; c4=*s++;
                TRICE8_4( Id(57970), "%c%c%c%c", c1, c2, c3, c4 ); return;
            case  5: c1=*s++; c2=*s++; c3=*s++; c4=*s++; c5=*s++;
                TRICE8_5( Id(49813), "%c%c%c%c%c", c1, c2, c3, c4, c5 ); return;
            case  6: c1=*s++; c2=*s++; c3=*s++; c4=*s++; c5=*s++; c6=*s++;
                TRICE8_6( Id(10201), "%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6 ); return;
            case  7: c1=*s++; c2=*s++; c3=*s++; c4=*s++; c5=*s++; c6=*s++; c7=*s++;
                TRICE8_7( Id(57439), "%c%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6, c7); return;
            case  8:
            default: c1 = *s++; c2 = *s++; c3 = *s++; c4 = *s++; c5 = *s++; c6 = *s++; c7 = *s++; c8 = *s++; 
                TRICE8_8( Id(53018), "%c%c%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6, c7, c8 );
                len -= 8;
        }
    }
    return;
}
#endif 

#if RARE_RUNTIME == TRICE_STRINGS

//! trice a string
//!\details not very effective but better than no strings for now
//!This function could be useful, if the string is generated dynamically.
//!\param s 0-terminated string
void triceString( int rightBound, const char* s ){
    size_t len = strlen( s );
    int spaces = rightBound - len;
    spaces = spaces < 0 ? 0 : spaces;
    TRICE_ENTER_CRITICAL_SECTION
    triceSpaces( spaces );
    triceStringUnbound( s );
    TRICE_LEAVE_CRITICAL_SECTION
}

#endif

#if FULL_RUNTIME == TRICE_STRINGS

static uint8_t Cycle( void ){
    static uint8_t cycle = 0;
    cycle++;
    cycle &= 0x3F; // 0011.1111 -> 6 bit counter
    return cycle;
}

///////////////////////////////////////////////////////////////////////////////
// fifo instance
//

typedef struct{
    size_t size; //!< fifo data size
    uint8_t* pBuff; //!< start address
    uint8_t* pLimit; //!< first address behind fifo data
    uint8_t* pWr; //!< internal write pointer
    uint8_t* pRd; //!< internal read pointer
} Fifo_t; //!< first in first out struct type

#define WSIZ RUNTIME_STRING_FIFO_SIZE //!< must not nessesaryly be power of 2!
static uint8_t rcWr[ WSIZ ]; 

//! fifo control struct, UART transmit date are provided here
static Fifo_t wrFifo = {WSIZ, rcWr, rcWr+WSIZ, rcWr, rcWr }; 

///////////////////////////////////////////////////////////////////////////////
// fifo access
//

//! make sure write pointer stays inside fifo range
//! \param f pointer to fifo struct
static void fifoLimitateWrPtr_Unprotected( Fifo_t* f ){
    f->pWr -= f->pWr < f->pLimit ? 0 : f->size;
}



//! make sure read pointer stays inside fifo range
//! \param f pointer to fifo struct
static void fifoLimitateRdPtr_Unprotected( Fifo_t* f ){
    f->pRd -= f->pRd < f->pLimit ? 0 : f->size;
}

//! \param f pointer to fifo struct
//! \param count a computed count (could be negative)
//! \return count if count was positive or with size corrected value as correct count
static inline size_t adjustCount( Fifo_t const * const f, int count ){
    count = count < 0 ? count + (unsigned)(f->size) : count; //lint !e713 !e737
    return (size_t)count;
}


//! fifo depth
//! \details a fifo of size n (n>1!) can hold only n-1 values to be able to distinct between empty and full
//! \param f pointer to fifo struct
//! \retval Count of bytes actually in fifo
static size_t FifoReadableCount_Unprotected( Fifo_t const * const f ){
    size_t n = adjustCount( f, (int)(f->pWr - f->pRd) );
    return n;
}

//! Get an uint8_t value from fifo
//! \param f pointer to fifo struct
//! \param pValue address where to store value
static void FifoPopUint8_Unprotected( Fifo_t* f, uint8_t* pValue ){
    *pValue = *(f->pRd);
    f->pRd++;
    fifoLimitateRdPtr_Unprotected(f);
}



//! return amount of bytes continuously can be written without write pointer wrap
//! This is __not__ the free writable count! See also FifoWritableCount().
//! \param f pointer to fifo struct
//! \retval size of bytes which can be continuously written without write pointer wrap
static inline size_t FifoWritableBlockSpace( const Fifo_t* f ){
    return (size_t)(f->pLimit - f->pWr);
}

//! Push count bytes into fifo. The value count must not be bigger than FifoWritableCount() and is not checked!
//! \param f pointer to fifo struct
//! \param count buffer size
//! \param pBuff address of buffer to be pushed into fifo
static void FifoPushBuffer_Unprotected( Fifo_t* f, size_t count, const uint8_t* pBuff ){
    size_t size = FifoWritableBlockSpace(f);
    if( size >= count ){ // 
        memcpy( f->pWr, pBuff, count );
    }else{ // 
        memcpy( f->pWr, pBuff, size );
        memcpy( f->pBuff, pBuff+size, count-size );
    }
    f->pWr += count;
    fifoLimitateWrPtr_Unprotected(f);
}

///////////////////////////////////////////////////////////////////////////////
// tx 
//

//! transfer special com buffer "trice string buffer" to write fifo
//! \param string len, max valid value is 65536, len 0 means no transmission at all
//! \param s pointer to string
//! \return used packet index 
//! \code
//! trice package: header without data packages
//!   |--------------------------------- fixed packet start0 byte 0xeb 
//!   |   |----------------------------- client address (local address byte)
//!   |   |   |------------------------- server address (destination)
//!   |   |   |   |--------------------- exclusive-or checksum byte
//!   |   |   |   |   |----------------- ID low part
//!   |   |   |   |   |   |------------- ID high part
//!   |   |   |   |   |   |   |--------- Value Low part
//!   |   |   |   |   |   |   |   |----- Value High part
//!   v   v   v   v   v   v   v   v
//! 0xeb cad sad cr8 idL idH vaL  vaH
//! 
//! com packet: header followed by 0...255 data packages
//!   |--------------------------------- fixed packet start0 byte 0xc0 
//!   |   |----------------------------- following data package count fixed 1 for trice strings
//!   |   |   |------------------------- server address (destination)
//!   |   |   |   |--------------------- exclusive-or checksum byte
//!   |   |   |   |   |----------------- type identifyer byte
//!   |   |   |   |   |   |------------- function identifyer byte
//!   |   |   |   |   |   |   |--------- packet index (2 lsb packet type and and 6 msb cycle counter)
//!   |   |   |   |   |   |   |   |----- data package count
//!   v   v   v   v   v   v   v   v
//! 0xc0 cad sad cr8 tid fid pix dpc 
//! 
//! com type: (part of pix) 
//!       bit1      |      bit0       | meaning
//!   COM_CMD_FLAG  | COM_ANSWER_FLAG |
//! ----------------|-----------------|------------------------------------------
//!         1       |        1        | \b Cmd = command expecting answer
//!         0       |        1        | \b Ans = answer to a command expecting answer
//!         1       |        0        | \b Msg = command not expecting an answer (message)
//!         0       |        0        | \b Buf = trice string package, when 0xffff==tidfid
//! \endcode
static uint8_t triceReCalStringBuffer( size_t len, const char* s ){
    uint16_t len_1 = (uint16_t)(len-1); // len-1 is transmitted in data package
    #define CAD TRICE_LOCAL_ADDR // client address
    #define SAD TRICE_DISPL_ADDR // server address
    #define TID 0xff // type ID, here fixed to 0xFF for string package identification
    #define FID 0xff // type ID, here fixed to 0xFF for string package identification
    #define DPC 1    // exact one data package here  
    #define CR8                   (0xc0  ^CAD ^SAD      ^TID ^FID      ^DPC) // partial ex-or crc8
    enum{                            c0,  cad, sad, cr8, tid, fid, pix, dpc, lenL, lenH }; // header plus length
    // index                          0    1    2    3    4    5    6    7    8     9
    ALIGN4 static uint8_t h[10] = { 0xc0, CAD, SAD,  0,  TID, FID,  0,  DPC,  0,    0 } ALIGN4_END;
    h[pix] = Cycle(); // package index, 6 bit cycle counter, 2 msb = 0 for special package identification, TIDFID==0xffff = string package
    h[cr8] = CR8 ^ h[pix];
    h[lenL] = (uint8_t)len_1;
    h[lenH] = (uint8_t)(len_1>>8);
    
    if( 0 == len || 65536 < len ){
        TRICE32_1( Id( 9285), "ERR:invalid length %d, ignoring trice string package\n", len );
        return 0;
    }

    // first send buffer data, buffer data have transmission priority, so they are in place when needed
    TRICE_ENTER_CRITICAL_SECTION
    FifoPushBuffer_Unprotected( &wrFifo, sizeof(h), h ); // header is 8 byte and we add the 2 len bytes in one shot 
    FifoPushBuffer_Unprotected( &wrFifo, len, (uint8_t*)s );
    TRICE_LEAVE_CRITICAL_SECTION
    return h[pix];
}

static void triceStringUnbound( const char* s ){
    size_t len = strlen( s );
    uint8_t pix = triceReCalStringBuffer( len, s );
    // a short TRICE with "%s" and the index number together follows the separate special com packet sequence
    TRICE8_1( Id( 8479), "%s", pix ); // "%s" tells trice tool that a separate string package was sent, pix (cycle) is used for string identification
}

//! trice a string
//!\details not very effective but better than no strings for now
//!This function could be useful, if the string is generated dynamically.
//!\param s 0-terminated string
void triceString( int rightBound, const char* s ){
    TRICE_ENTER_CRITICAL_SECTION
    size_t len = strlen( s );
    int spaces = rightBound - len;
    spaces = spaces < 0 ? 0 : spaces;
    triceSpaces( spaces );
    triceStringUnbound( s );
    TRICE_LEAVE_CRITICAL_SECTION
}



static int pkgByteIdx = -1; //!< helper for stream interpretation

//! start a new package header
static inline void resetPkgByteIndex( void ){
    pkgByteIdx = -1; 
}



//! get netx byte from package fifo and transfer it 
//! \return transferred byte
static uint8_t txNextByte( void ){
    uint8_t v;
    FifoPopUint8_Unprotected( &wrFifo, &v );
    triceTransmitData8( v );
    return v;
}


//! comTX starts a com transmission if possible, otherwise it does nothing
//! It checks these conditions:
//! \li currently no transmission: TxState is noTx
//! \li tx register is empty 
//! \li wrFifo contains at least one com Header
//! In that case the TxState changes from noTx to comTx and the first byte goes out
//! and the transmit empty interrupt is enabled.
//! Otherwise comTxStart does nothing.
//! on continue it checks if the tx register is empty and that the TxState is comTx flag and does nothing otherwise
//! txState set to noTx when current packet done
static void comTX( void ){
    static uint8_t dpc = 0;     // we must transfer a complete block without trices inbetween
    static uint32_t len = 0;    // 0, 1...65536 must fit
    static uint8_t lenL = 0;
    
    if( !triceTxDataRegisterEmpty() ){ // comTX and triceTx must check that independently
        return;
    }
    
    if( noTx == txState && 10 <= FifoReadableCount_Unprotected( &wrFifo ) ){ 
        // start only if a full header plus following len already in wrFifo.
        // This is safe, because following data will be in fifo before the first 10 bytes are transmitted
        txState = comTx;
        triceEableTxEmptyInterrupt(); 
    }
    
    if( comTx != txState ){
        return;
    }
    // continue
    pkgByteIdx++; // reset value is -1, so starts here with 0
    if( pkgByteIdx < 7 ){ // 0xc0 cad sad cr8 tid fid pix dpc
        txNextByte();     //   0   1   2   3   4   5   6   7
        return;
    }
    if ( 7 == pkgByteIdx ){ // dpc
        dpc = txNextByte(); // tx last byte of tx header 
        if( 1 != dpc ){ // unexpected value
            TRICE8_1( Id( 1041), "err: dpc=%d\n", dpc );
            resetPkgByteIndex();
            return;
        }
        if( 0 == dpc ){ 
            resetPkgByteIndex(); // reload for next package
            triceDisableTxEmptyInterrupt();
            txState = noTx; // com packet done
        }
        return;
    }

    if ( 8 == pkgByteIdx ){ // dpc is > 0, so read len first time
        lenL = txNextByte();
        return;
    }
    if ( 9 == pkgByteIdx ){
        len = txNextByte() << 8; // little endian
        len |= lenL;
        len++; // len is len-1 value
        return;
    }

    // "loop" starts here
    if ( pkgByteIdx < len + 9 ){ // len=1...65536
        txNextByte(); // tx data
        return;
    } else if ( pkgByteIdx == len + 9 ){ // len=1...65536
        txNextByte(); // tx last date in data package
        dpc--;
        if( 0 == dpc ){ 
            resetPkgByteIndex();
            triceDisableTxEmptyInterrupt();
            txState = noTx; // com packet done
        }
        return;
    } 
    
    TRICE0( Id(62952), "ATT:this point is reached when dpc was bigger 1, read len of following data package\n" );
    if ( len + 10 == pkgByteIdx ){ // 10 = header + sizeof(uint16_t)
        lenL = txNextByte();
        return;
    } 
    if ( len + 11 == pkgByteIdx ){
        int len_1 = len; // keep old value
        len = txNextByte() << 8;
        len |= lenL;
        len ++; // len is len-1 value
        len = len_1 + sizeof(uint16_t) + len; // accumulate
        return;
    }
}

#endif // #if FULL_RUNTIME == TRICE_STRINGS



#if defined(TRICE_WRITE_OUT_FUNCTION) &&  TRICE_WRITE_OUT_FUNCTION == STM32_LLDRV

//! Check if a new byte can be written into trice transmit register.
//! \retval 0 == not empty
//! \retval !0 == empty
//! User must provide this function.
TRICE_INLINE uint32_t triceTxDataRegisterEmpty( void ){
    return LL_USART_IsActiveFlag_TXE( TRICE_UART );
}

TRICE_INLINE int triceWrite( char* c, int count ){
	if( count && triceTxDataRegisterEmpty() ){
		LL_USART_TransmitData8( TRICE_UART, *c); // only one char 
	}
	return count - 1;
}




TRICE_INLINE int triceWrite( char* c, int count ){
	if( count && triceTxDataRegisterEmpty() ){
		LL_USART_TransmitData8( TRICE_UART, *c); // only one char 
	}
	return count - 1;
}

#endif // #if defined(TRICE_WRITE_OUT_FUNCTION) &&  TRICE_WRITE_OUT_FUNCTION == STM32_LLDRV



#ifdef TRICE_USE_WRITE_FUNCTION
/*
void triceServeTransmit( void ){
    size_t sent;
    static size_t remaining = 0; // amount of not transmitted bytes inside triceMsg
    if( remaining ){ // try to transmit rest data
        uint8_t * rd = (uint8_t*)limit - remaining; // next read address
        sent = triceWrite( rd, remaining ); 
        remaining -= sent;
        return;
    }else if( triceFifoDepth() ){
        prepareNextTriceTransmission();
        sent = triceWrite( pRead, sizeof(triceMsg) ); 
        remaining = sizeof(triceMsg) - sent;
    }
}*/

void triceServeTransmit( void ){
    size_t sent;
    static size_t remaining = 0; // amount of not transmitted bytes inside triceMsg
    if( remaining ){ // try to transmit rest data
        uint8_t * rd = (uint8_t*)limit - remaining; // next read address
        sent = triceWrite( rd, remaining ); 
        remaining -= sent;
        return;
    }else if( triceFifoDepth() ){
        prepareNextTriceTransmission();
        sent = triceWrite( pRead, sizeof(triceMsg) ); 
        remaining = sizeof(triceMsg) - sent;
    }
}


// how to implement a non-blocking write, which usually succeeds


//! 
static void triceTX( void ){
    if( (noTx == txState) &&  triceNextMsg() ){ 
            txState = triceTx; 
            triceWrite( &triceMsg, 8 ); // immediate return
    }

#endif // #ifdef TRICE_USE_WRITE_FUNCTION


//! This function should be called inside the transmit done device interrupt.
//! Also it should be called cyclically to trigger transmission start.
//!         // check if data in trice fifo and load them in the 8 byte transmit buffer, optionally encrypted
//!         // it is possible SysTick comes immediately after Uart ISR and the register is not empty yet
//! internal using transmission state variable txState. It is cleared if no more traceLog messages to transmit and set to 1 if a traceLog transmission was started.
//! \todo handle 8==traceLogMsgDepth() to give chance to other data streams
//! transmit one trice msg byte
//! internal using transmission state variable txState. It is cleared if no more traceLog messages to transmit and set if a traceLog transmission was started.
//! \todo handle 8==traceLogMsgDepth() to give chance to other data streams
static void triceTX( void ){
    if( !triceTxDataRegisterEmpty() ){ // // comTX and triceTx must check that independently
        return;
    }
    if( (noTx == txState) &&  triceNextMsg() ){ 
            txState = triceTx; 
    }
    if( (triceTx == txState) ){ 
        if( triceMsgBufferDepth() ){
            uint8_t x = triceMsgNextByte();
            triceTransmitData8( x );
            triceEableTxEmptyInterrupt();
        }else{
            triceDisableTxEmptyInterrupt();
            txState = noTx;
        }
    }
}


//! Check for data and start a transmission, if both channes have data give priority to com.
//! should be activated cyclically for example every 1 ms for small transmit delays

//! continue ongoing transmission, otherwise check for new data transmission
//! should be activated cyclically for example every 1 ms for small transmit delays

//! This function should be called inside the transmit done device interrupt.
//! Also it should be called cyclically to trigger transmission start, for example in the sysTick interrupt.
//! If not interrup is used it should be called cyclically. With each call max 1 byte is transmitted
void TriceServeTransmission( void ){
#if FULL_RUNTIME == TRICE_STRINGS
    comTX(); // com transmission comes first to have priority over trices - iportand for 2 reasons: 
    // 1. runtime strings are earlier before their trigger trices.
    // 2. com is more time critical than trice transmission (in case of RPC usage)
#endif
    triceTX();
}

#endif // #else // #if NO_CODE == TRICE_CODE
