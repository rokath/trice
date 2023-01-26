//! \file messages.h
//! \author thomas.hoehenleitner [at] seerose.net

#include <stdint.h>

int Message0( uint8_t b0 );
int Message1( uint16_t stamp, uint8_t b0 );
int Message2( uint32_t stamp, uint8_t b0 );
