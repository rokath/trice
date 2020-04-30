/*! \file triceRuntimeGeneratedStrings.c
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#include "trice.h"

#if TRICE_CODE

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

#endif
