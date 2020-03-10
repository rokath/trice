/*! \file trice.c
\brief trices to transfer buffer functionality
\details The trices are dumped as 32bit values into a 32 bit fifo.
That is the time critical part. 
\li a trice is a 16 bit ID with a 16 bit data value
\li trices with more data are split into several 32bit basic trice values with IDs = 0 in front.
\li So a basic trice (subtrace) consists always of 4 byte.
\li for trice transmission each basic trice gets a header of additional 4 bytes.
\li The header contains a startbyte, client and server address and a crc8 exOr checksum.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#include "trice.h"
#include "fifo.h"
#ifdef ENCRYPT
#include "xteaCrypto.h" // enable this for encryption
#endif

#ifdef TRICE_PRINTF_ADAPTER
#include <stdarg.h>
#include <stdio.h> // #include "printf.h"
#endif // #ifdef TRICE_PRINTF_ADAPTER

#if 0 == TRICE_LEVEL
void triceTxHandler( int* pTxState ){
}

int tricePrintfAdapter( const char* pFmt, ... ){
}
#else // #if 0 == TRICE_LEVEL

enum{
    noTx, // no transmission
    triceTx, // trice packet in transmission
    reCalTx // remote call packet in transmission
};

static int txState = noTx; //!< txState, needed to not mix the channel data

//! trice fifo instance
ALIGN4 uint32_t triceFifo[ TRICE_FIFO_SIZE>>2 ] ALIGN4_END;

uint32_t rdIndexTriceFifo = 0; //!< trice fifo read index
uint32_t wrIndexTriceFifo = 0; //!< trice fifo write index

//! partial prefilled trice message transmit buffer 
ALIGN4 static triceMsg_t triceMsg ALIGN4_END = {
    { TRICE_START_BYTE,  TRICE_LOCAL_ADDR,  TRICE_DISPL_ADDR, 0 }, // crc8
    { 0, 0 } // 16bit ID, 16bit data
};

static uint8_t const * const limit = (uint8_t*)(&triceMsg + 1); //!< trice message buffer limit
static uint8_t       *       pRead = (uint8_t*)(&triceMsg + 1); //!< trice message buffer read pointer

//! get next trice byte for transmission from trice message buffer, no depth check here
//!\retval data byte
TRICE_INLINE uint8_t triceMsgNextByte( void ){
    return *pRead++;
}

static size_t triceMsgBufferDepth( void ){
    size_t count = limit - pRead;
    return count;
}

//! prepare next trice for transmission
//!\retval 0 no next trice
//!\retval 1 next trice in message buffer
static size_t triceNextMsg( void ){
    if( triceFifoDepth() ){
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
        return 1;
    }
    return 0;
}


/*! This function should be called inside the transmit done device interrupt.
Also it should be called cyclically to trigger transmission start.
        // check if data in trice fifo and load them in the 8 byte transmit buffer, optionally encrypted
        // it is possible SysTick comes immediately after Uart ISR and the register is not empty yet
\param pTxState address of a transmission state variable. It is cleared if no more traceLog messages to transmit and set to 1 if a traceLog transmission was started.
\todo handle 8==traceLogMsgDepth() to give chance to other data streams
*/
void triceTxStart( int* pTxState ){
    if( (noTx == *pTxState) && triceTxDataRegisterEmpty() &&  triceNextMsg() ){ 
            *pTxState = triceTx;
            triceEableTxEmptyInterrupt(); 
    }
}

/*! This function should be called inside the transmit done device interrupt.
Also it should be called cyclically to trigger transmission start.
\param pTxState address of a transmission state variable. It is cleared if no more traceLog messages to transmit and set to 1 if a traceLog transmission was started.
\todo handle 8==traceLogMsgDepth() to give chance to other data streams
*/
void triceTxContinue( int* pTxState ){
    if( (triceTx == *pTxState) && triceTxDataRegisterEmpty() ){ 
        if( triceMsgBufferDepth() ){
            uint8_t x = triceMsgNextByte();
            triceTransmitData8( x ); 
        }else{
            triceDisableTxEmptyInterrupt();
            *pTxState = noTx;
        }
    }
}

#ifdef TRICE_PRINTF_ADAPTER // inside #else // #if 0 == TRICE_LEVEL

void triceString( int rightBound, const char* s );

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

#ifdef TRICE_SHORT_MEMORY

void triceSpaces( int spaces ){
    while( spaces-->0 )
    {
        TRICE0( Id(27950), " " );
    }
}

#else // #ifdef TRICE_SHORT_MEMORY

void triceSpaces( int spaces ){
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
#endif // #else // #ifdef TRICE_SHORT_MEMORY

#ifndef LONG_RUNTIME_STRINGS // inside #ifdef TRICE_PRINTF_ADAPTER

#ifdef TRICE_SHORT_MEMORY

void triceStringUnbound( const char* s ){
    while( *s )
    {
        TRICE8_1( Id(3), "%c", *s );
        s++;
    }
}

#else // #ifdef TRICE_SHORT_MEMORY

//! for performance no check of strlen( s ) here (internal usage)
static inline void triceStringN( size_t len, const char* s ){
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

void triceStringUnbound( const char* s ){
    size_t len = strlen( s );
    triceStringN( len, s );
}
#endif // #else // #ifdef TRICE_SHORT_MEMORY


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

#else // #ifndef LONG_RUNTIME_STRINGS inside #ifdef TRICE_PRINTF_ADAPTER

static uint8_t Cycle( void ){
    static uint8_t cycle = 0;
    cycle++;
    cycle &= 0x3F; // 0011.1111 -> 6 bit counter
    return cycle;
}


///////////////////////////////////////////////////////////////////////////////
// fifo instances
//

#define WSIZ RUNTIME_STRING_FIFO_SIZE //!< must not nessesaryly be power of 2!
static uint8_t rcWr[ WSIZ ]; 

//! fifo control struct, UART transmit date are provided here
static Fifo_t wrFifo = {WSIZ, rcWr, rcWr+WSIZ, rcWr, rcWr }; 

///////////////////////////////////////////////////////////////////////////////
// tx 
//

//! transfer special reCal buffer "trice string buffer" to write fifo
//! \param string len, max valid value is 65536, len 0 means no transmission at all
//! \param s pointer to string
//! \return used packet index 
static uint8_t triceReCalStringBuffer( size_t len, const char* s ){
    uint16_t len_1 = (uint16_t)(len-1); // len-1 is transmitted in data package
    #define CAD 0x60 // client address
    #define SAD 0x60 // server address
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
    // a short TRICE with "%s" and the index number together follows the separate special reCal packet sequence
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

//! reCalTxHandler starts a reCal transmission if possible, otherwise it does nothing
//! It checks these conditions:
//! \li currently no transmission: TxState is noTx
//! \li tx register is empty 
//! \li wrFifo contains at least one reCal Header
//! In that case the TxState changes from noTx to reCalTx and the first byte goes out
//! and the transmit empty interrupt is enabled.
//! Otherwise reCalTxStart does nothing.
//! \param pTxState pointer to TxState
static void reCalTxStart( int* pTxState ){
    if( noTx == *pTxState && triceTxDataRegisterEmpty() && 8 <= FifoReadableCount_Unprotected( &wrFifo ) ){ 
        // start only if a full header already in wrFifo.
        // This is safe, because following data will be in fifo before the first 8 bytes are transmitted
        *pTxState = reCalTx;
        triceEableTxEmptyInterrupt(); 
    }
}

//! reCalTxContinue checks if the tx register is empty and that the TxState is reCalTx flag and does nothing otherwise
//! \param pTxState pointer to TxState, TxState set to noTx when current packet done
static void reCalTxContinue( int* pTxState ){
    static uint8_t dpc = 0;     // we must transfer a complete block without trices inbetween
    static uint32_t len = 0;    // 0, 1...65536 must fit
    static uint8_t lenL = 0;

    if( reCalTx != *pTxState || !triceTxDataRegisterEmpty() ){
        return;
    }

    pkgByteIdx++; // reset value is -1, so starts here with 0
    if( pkgByteIdx < 7 ){ // 0xc0 cad sad cr8 tid fid pix dpc
        txNextByte();     //   0   1   2   3   4   5   6   7
        return;
    }
    if ( 7 == pkgByteIdx ){ // dpc
        dpc = txNextByte(); // tx last byte of tx header 
        if( 1 != dpc ){
            TRICE8_1( Id( 1041), "err: dpc=%d\n", dpc );
           resetPkgByteIndex();
            return;
        }
        if( 0 == dpc ){ 
           resetPkgByteIndex(); // reload for next package
            triceDisableTxEmptyInterrupt();
            *pTxState = noTx; // reCal packet done
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
            *pTxState = noTx; // reCal packet done
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

#endif // #else // #ifndef LONG_RUNTIME_STRINGS inside #ifdef TRICE_PRINTF_ADAPTER

#endif // #ifdef LONG_RUNTIME_STRINGS

//! Check for data and start a transmission, if both channes have data give priority to reCal.
//! \param pTxState pointer to TxState, do nothing if not noTx
//! should be activated cyclically for example every 1 ms for small transmit delays
void TxStart( void ){
#ifdef LONG_RUNTIME_STRINGS
    reCalTxStart( &txState ); // reCal transmission has priority over trices - iportand for 2 reasons: 
    // 1. runtims strings are earlier before their trigger trices.    
    // 2. reCal is more time critical than trice transmission
#endif // #ifdef TRICE_PRINTF_ADAPTER
    triceTxStart( &txState );
}

//! continue ongoing transmission, otherwise check for new data transmission
//! \param pTxState actual transmission state
//! should be activated cyclically for example every 1 ms for small transmit delays
void TxContinue( void ){
#ifdef LONG_RUNTIME_STRINGS
    reCalTxContinue( &txState );
#endif // #ifdef TRICE_PRINTF_ADAPTER
    triceTxContinue( &txState );
}

#endif // #if 0 == TRICE_LEVEL


//! unused dummy definition for linker
void _putchar(char character){
}


// #define RSIZ 4 //!< must be power of 2!
// static uint8_t rcRd[ RSIZ ];
// 
// //! fifo control struct, UART received data are arriving here
// Fifo_t rdFifo = {RSIZ, rcRd, rcRd+RSIZ, rcRd, rcRd }; 

