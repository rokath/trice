/*! \file triceRuntimeGeneratedStrings.c
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#include "trice.h"

#if TRICE_CODE

#ifdef TRICE_RUNTIME_GENERATED_STRINGS_SUPPORT

void triceRuntimeGeneratedSpaces( int spaces ){
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

//! for performance no check of strlen( s ) here (internal usage)
static void triceRuntimeGeneratedStringUnbound( const char* s ){
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

//! trice a string
//!\details not very effective but better than no strings for now
//!This function could be useful, if the string is generated dynamically.
//!\param s 0-terminated string
void triceRuntimeGeneratedString( int rightBound, const char* s ){
    size_t len = strlen( s );
    int spaces = rightBound - len;
    spaces = spaces < 0 ? 0 : spaces;
    TRICE_ENTER_CRITICAL_SECTION
    triceRuntimeGeneratedSpaces( spaces );
    triceRuntimeGeneratedStringUnbound( s );
    TRICE_LEAVE_CRITICAL_SECTION
}

#endif // #ifdef TRICE_RUNTIME_GENERATED_STRINGS_SUPPORT

#ifdef TRICE_RUNTIME_GENERATED_STRINGS_SUPPORT_EXPERIMENTAL

#ifdef ENCRYPT
#error todo: encryption does not yet work with this module
#endif

//! Create a new cycle number
//! \return next cycle number
static uint8_t Cycle( void ){
    static uint8_t cycle = 0;
    cycle++;
    cycle &= 0x3F; // 0011.1111 -> 6 bit counter
    return cycle;
}

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

//! trice a string
//!This function can handle dynamically generated strings
//!\param s 0-terminated string
static void triceRuntimeGeneratedStringUnbound( const char* s ){
    size_t len = strlen( s );
    uint8_t pix = triceReCalStringBuffer( len, s );
    // a short TRICE with "%s" and the index number together follows the separate special com packet sequence
    TRICE8_1( Id( 8479), "%s", pix ); // "%s" tells trice tool that a separate string package was sent, pix (cycle) is used for string identification
}

//! trice a space string
//!\param spaces count of spaces
static void triceRuntimeGeneratedSpaces( int spaces ){
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

//! trice a string right bounded
//!This function can handle dynamically generated strings
//! \param rightBound position of end of string from current out position
//!\param s 0-terminated string
void triceRuntimeGeneratedString( int rightBound, const char* s ){
    TRICE_ENTER_CRITICAL_SECTION
    size_t len = strlen( s );
    int spaces = rightBound - len;
    spaces = spaces < 0 ? 0 : spaces;
    
    triceRuntimeGeneratedSpaces( spaces );
    triceRuntimeGeneratedStringUnbound( s );
    TRICE_LEAVE_CRITICAL_SECTION
}

#endif // #ifdef #ifdef TRICE_RUNTIME_GENERATED_STRINGS_SUPPORT_EXPERIMENTAL

#endif // #if TRICE_CODE
