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
#include "triceWrite.h"
#ifdef ENCRYPT
#include "xteaCrypto.h"
#endif

#ifdef TRICE_PRINTF_ADAPTER
#include <stdarg.h>
#include <stdio.h> 

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

#if NO_CODE == TRICE_CODE

#error

#else // #if NO_CODE == TRICE_CODE

//! trice fifo instance, here are the trices buffered. used in TRICE macro expansion
ALIGN4 uint32_t triceFifo[ TRICE_FIFO_SIZE>>2 ] ALIGN4_END;

uint32_t wrIndexTriceFifo = 0; //!< trice fifo write index, used inside macros, so must be visible

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

//! pull next trice from fifo and prepare triceMsg buffer
static void prepareNextTriceTransmission(void){
    triceFifoPop( (uint32_t*)(&(triceMsg.ld)) );
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

uint32_t maxTrices = 0;

static void triceTX( void ){
    uint32_t depth = triceFifoDepth();
    if( depth ){
        #ifdef USE_OWN_TRICE_WRITE_FUNCTION
        if( writeCount >= TRICE_WRITE_COUNT_LIMIT){
            return; // don't transfer trices if too buffer is too full
        }
        #endif
        prepareNextTriceTransmission();
        triceWrite( &triceMsg, sizeof(triceMsg) );
        maxTrices = depth < maxTrices ? maxTrices : depth;
    }
}

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

#endif // #if RARE_RUNTIME == TRICE_STRINGS

#if FULL_RUNTIME == TRICE_STRINGS

static uint8_t Cycle( void ){
    static uint8_t cycle = 0;
    cycle++;
    cycle &= 0x3F; // 0011.1111 -> 6 bit counter
    return cycle;
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
static uint8_t triceReCalStringBuffer( size_t len, const char*  s ){
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
    triceWrite( h, sizeof(h) ); // header is 8 byte and we add the 2 len bytes in one shot 
    triceWrite( s, len );
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

#endif // #if FULL_RUNTIME == TRICE_STRINGS

//! This function should be called inside the transmit done device interrupt.
//! Also it should be called cyclically to trigger transmission start, for example in the sysTick interrupt.
//! If not interrup is used it should be called cyclically. With each call max 1 byte is transmitted.
void TriceServeTransmission( void ){
    triceWriteServer();
    triceTX();
}

#endif // #else // #if NO_CODE == TRICE_CODE
