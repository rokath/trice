/*! \file triceSEncoding.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_S_ENCODING_H_
#define TRICE_S_ENCODING_H_

#ifdef __cplusplus
extern "C" {
#endif


//! string transfer format: 
//!     id           count <--- both values in trice transfer order
//! c0     c1     c2       c3
//! ...
//! cLen-3 cLen-2 cLen-1   cLen
//! \todo code needs to be checked on a big endian machine.
TRICE_INLINE void trice_s(uint32_t id, char *s) {
    TRICE_ENTER_CRITICAL_SECTION
    int i = 0;
    int len = strlen( s );
    if( 65535 < len ){
        s[65535] = 0; // truncate
        len = 65535;
    }
    TRICE_HTON_U32PUSH( id|len ); // on PC side the id reception gives the TRICE_S and the format string information
    while( 3 < len ){
        uint32_t* pos = (uint32_t*)(s+i);
        TRICE_U32PUSH( *pos );
        //TRICE_HTON_U32PUSH( ((uint32_t)(s[i+0]) << 24)|((uint32_t)(s[i+1]) << 16)|((uint32_t)(s[i+2]) << 8)|(uint32_t)(s[i+3]) );
        len -= 4;
        i += 4;
    }
    switch( len ){
        case 0: 
            break;
        case 1: 
            TRICE_U32PUSH( s[i+0] );
            break;
        case 2: 
            TRICE_U32PUSH( ((s[i+1]<<8)|s[i+0]) );
            break;
        case 3: 
            TRICE_U32PUSH( (s[i+2]<<16) | (s[i+1]<<8) | s[i+0] );
            break;
    }
    TRICE_LEAVE_CRITICAL_SECTION
}

//! Write id and dynString protected (outside critical section).
//! \param id trice identifier
//! \param pFmt formatstring for trice (ignored here but used by the trice tool)
//! \param dynString 0-terminated runtime generated string
//! After the 4 byte trice message header are following 2^n bytes 
#define TRICE_S(id, pFmt, dynString) do{ trice_s(id, dynString); }while(0)

#ifdef __cplusplus
}
#endif

#endif /* TRICE_S_ENCODING_H_ */
