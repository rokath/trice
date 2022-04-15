/*! \file trice_test.c
\brief wrapper for tests
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#include <stdint.h>

void SetTriceBuffer( uint8_t* buf );
int T1( void );
int T2( void );

extern int triceBufferDepth;
extern uint8_t* triceBuffer;
