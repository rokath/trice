
#include "trice.h"

#if TRICE_MODE == TRICE_STACK_BUFFER

#if TRICE_SINGLE_MAX_SIZE + TRICE_DATA_OFFSET > TRICE_STACK_BUFFER_MAX_SIZE
#error
#endif


#if defined(TRICE_STACK_BUFFER_MAX_SIZE) && !defined(TRICE_SINGLE_MAX_SIZE)
#define TRICE_SINGLE_MAX_SIZE (TRICE_STACK_BUFFER_MAX_SIZE - TRICE_DATA_OFFSET)
#endif


//! TriceDepthMax returns the max trice buffer depth until now.
size_t TriceDepthMax( void ){
    return triceDepthMax;
}

void TriceLogBufferInfo( void ){
    TRICE32( Id(13778), "att:Single Trice Stack buf size:%4u", TRICE_SINGLE_MAX_SIZE + TRICE_DATA_OFFSET );
}

#if defined( TRICE_UART ) // direct out to UART
void TriceWrite( uint8_t const * buf, unsigned len ){
    TriceBlockingWrite( buf, len );
}
#endif

#endif // #if TRICE_MODE == TRICE_STACK_BUFFER
