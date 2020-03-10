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

#include <stdint.h>

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

/*! get next trice byte for transmission from trice message buffer, no depth check here
\retval data byte
*/
TRICE_INLINE uint8_t triceMsgNextByte( void ){
    return *pRead++;
}

static size_t triceMsgBufferDepth( void ){
    size_t count = limit - pRead;
    return count;
}

/*! prepare next trice for transmission
\retval 0 no next trice
\retval 1 next trice in message buffer
*/
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

#ifdef TRICE_PRINTF_ADAPTER

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

#endif // #ifdef TRICE_PRINTF_ADAPTER









#if 0 // 1 == TRICE_SHORT_MEMORY

//TRICE_INLINE 
void triceStringUnbound( const char* s ){
    while( *s )
    {
        TRICE8_1( Id(3), "%c", *s );
        s++;
    }
}

//TRICE_INLINE 
void triceString( int rightBound, const char* s ){
    size_t len = strlen( s );
    int spaces = rightBound - len;
    spaces = spaces < 0 ? 0 : spaces;
    TRICE_ENTER_CRITICAL_SECTION
    while( spaces-->0 )
    {
        TRICE0( Id(27950), " " );
    }
    triceStringUnbound( s );
    TRICE_LEAVE_CRITICAL_SECTION
}

// #else // #if 1 == TRICE_SHORT_MEMORY

// for performance no check of strlen( s ) here (internal usage)
//TRICE_INLINE 
void triceStringN( size_t len, const char* s ){
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

//TRICE_INLINE 
void triceStringUnbound( const char* s ){
    size_t len = strlen( s );
    triceStringN( len, s );
}

//TRICE_INLINE 
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


/*! trice a string
\details not very effective but better than no strings for now
This function could be useful, if the string is generated dynamically.
\param s 0-terminated string
*/
TRICE_INLINE void triceString( int rightBound, const char* s ){
    TRICE_ENTER_CRITICAL_SECTION
    size_t len = strlen( s );
    int spaces = rightBound - len;
    spaces = spaces < 0 ? 0 : spaces;
    triceSpaces( spaces );
    triceStringUnbound( s );
    TRICE_LEAVE_CRITICAL_SECTION
}

#else // #if 1 == TRICE_SHORT_MEMORY






//TRICE_INLINE 
static uint8_t Cycle( void ){
    static uint8_t cycle = 0;
    cycle++;
    cycle &= 0x3F; // 0011.1111 -> 6 bit counter
    return cycle;
}


//! transfer special reCal buffer "trice string buffer" to write fifo
//! \param string len, max valid value is 65536, len 0 means no transmission at all
//! \param s pointer to string
//! \return used packet index
//TRICE_INLINE 
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
#endif

//TRICE_INLINE 
static void triceStringUnbound( const char* s ){
    size_t len = strlen( s );
    uint8_t pix = triceReCalStringBuffer( len, s );
    // a short TRICE with "%s" and the index number together follows the separate special reCal packet sequence
    TRICE8_1( Id( 8479), "%s", pix ); // "%s" tells trice tool that a separate string package was sent, pix (cycle) is used for string identification
}

//TRICE_INLINE 
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


/*! trice a string
\details not very effective but better than no strings for now
This function could be useful, if the string is generated dynamically.
\param s 0-terminated string
*/
//TRICE_INLINE
void triceString( int rightBound, const char* s ){
    TRICE_ENTER_CRITICAL_SECTION
    size_t len = strlen( s );
    int spaces = rightBound - len;
    spaces = spaces < 0 ? 0 : spaces;
    triceSpaces( spaces );
    triceStringUnbound( s );
    TRICE_LEAVE_CRITICAL_SECTION
}

#endif // #else // #if 1 == TRICE_SHORT_MEMORY




















//! unused dummy definition for linker
void _putchar(char character){
}

//#endif // #else // #if 0 == TRICE_LEVEL
