/*! \file cgoTrice.c
\brief wrapper for tests
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include "trice.h"
#include "cgoTrice.h"

// cgoTriceBuffer is set to the (from Go) provided buffer address for the raw trice data.
uint8_t* cgoTriceBuffer;

// cgoTriceBufferDepth holds the number of valid raw trice bytes inside cgoTriceBuffer after TRICE macro execution.
int cgoTriceBufferDepth = 0;

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

//! cgoTriceStamp16 must be set before TRICE( Id(...), "...", ...) is called.
uint16_t cgoTriceStamp16 = 0x1616;

//! cgoTriceStamp32 must be set before TRICE( ID(...), "...", ...) is called.
uint32_t cgoTriceStamp32 = 0x32323232;

//! cgoTriceStamp64 must be set before TRICE( iD(...), "...", ...) is called.
uint64_t cgoTriceStamp64 = 0x6464646464646464;

// TriceStamp16 is used internally be the macro "Id"
uint16_t TriceStamp16( void ){
    return cgoTriceStamp16;
}

// TriceStamp16 is used internally be the macro "ID"
uint32_t TriceStamp32( void ){ 
    return cgoTriceStamp32; 
}

// TriceStamp64 is used internally be the macro "iD"
uint64_t TriceStamp64( void ){ 
    return cgoTriceStamp64; 
}
