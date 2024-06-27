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

// CgoClearTriceBuffer sets the internal cgoTriceBuffer cgoTriceBufferDepth to 0. 
// This function is called from Go for next test setup.
void CgoClearTriceBuffer( void  ){
    cgoTriceBufferDepth = 0;
}

//! TriceWriteDeviceCgo copies buf with len into triceBuffer.
//! This function is called from the trice runtime inside TriceWriteDevice().
void TriceWriteDeviceCgo( const void * buf, unsigned len ){
    memcpy(cgoTriceBuffer + cgoTriceBufferDepth, buf, len);
    cgoTriceBufferDepth += len;
}
