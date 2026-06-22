//! \file nodeAbc.h
//! Shared Trice ABC selection file for the TriceAbc demo nodes.
//! Generated once; afterwards intentionally kept user-owned.

#ifndef NODEABC_H_
#define NODEABC_H_

#include "triceRx.h"

#ifdef __cplusplus
extern "C" {
#endif

void logState(const triceRx_t* rx);
void setKey(const triceRx_t* rx);
void divide(const triceRx_t* rx);
void DivideResult(const triceRx_t* rx);
void getLeds(const triceRx_t* rx);
void setLeds(const triceRx_t* rx);
void LedsState(const triceRx_t* rx);

#ifdef __cplusplus
}
#endif

#endif /* NODEABC_H_ */
