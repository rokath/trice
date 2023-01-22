/*! \file cgoTrice.c
\brief wrapper for tests
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include <stdint.h>
#include <string.h>

// cgoTriceBuffer is set to the (from Go) provided buffer address for the raw trice data.
uint8_t* cgoTriceBuffer;

// cgoTriceBufferDepth holds the number of valid raw trice bytes inside cgoTriceBuffer after TRICE macro execution.
unsigned cgoTriceBufferDepth = 0;

// TriceOutDepthCGO provides access to the output buffer depth
unsigned TriceOutDepthCGO( void ){
    return cgoTriceBufferDepth;
}

// CgoSetTriceBuffer sets the internal cgoTriceBuffer pointer to buf. 
// This function is called from Go for test setup.
void CgoSetTriceBuffer( uint8_t* buf  ){
    cgoTriceBuffer = buf;
}

//! triceNonBlockingWriteCgoBuffer copies buf with len into triceBuffer.
//! This function is called from the trice runtime inside TriceWriteDevice().
void triceNonBlockingWriteCgoBuffer( uint8_t const * buf, unsigned len ){
    memcpy(cgoTriceBuffer, buf, len);
    cgoTriceBufferDepth = len;
}

// TriceStamp16 is used internally be the macro "Id"
uint16_t TriceStamp16( void ){
    return 0x1616;
}

// TriceStamp16 is used internally be the macro "ID"
uint32_t TriceStamp32( void ){ 
    return 0x32323232; 
}
