//! \file cobs.h
//! \author Thomas.Hoehenleitner [at] seerose.net

#ifndef COBS_H_
#define COBS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h> //lint !e537 !e451  Warning 537: Repeated include file,  Warning 451: Header file repeatedly included but does not have a standard
#include <stdint.h> //lint !e537 !e451  Warning 537: Repeated include file,  Warning 451: Header file repeatedly included but does not have a standard

//! If your compiler uses a pre-C99 C dialect and does not know The "__restrict" keyword, you can define it in the project settings.
size_t COBSEncode(void* __restrict out, const void* __restrict in, size_t length);

//! If your compiler uses a pre-C99 C dialect and does not know The "__restrict" keyword, you can define it in the project settings.
size_t COBSDecode(void* __restrict out, const void* __restrict in, size_t length);

#ifdef __cplusplus
}
#endif

#endif
