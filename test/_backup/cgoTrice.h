/*! \file cgoTrice.h
\brief wrapper for calling from Go
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#include <stdint.h>

void CgoSetTriceBuffer( uint8_t* buf );

extern int      cgoTriceBufferDepth;
extern uint8_t* cgoTriceBuffer;

extern uint16_t cgoTriceStamp16;
extern uint32_t cgoTriceStamp32;
