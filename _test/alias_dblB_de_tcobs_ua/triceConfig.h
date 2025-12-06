/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_AUXILIARY32 1
#define TRICE_CGO 1
#define TRICE_CYCLE_COUNTER 0

//////////////////////////////////////////////////////////////////////////////
// aliases - we have to exclude this in trice_environment.sh
//
#include "nanoprintf.h"

//! printi is a user print example with only integer values and therefore replacable direct with trice.
#define printi trice

//! prints is a user print example with string, float and integer values.
//! We have to perform a normal print into a buffer, which then is passed to triceS.
//! This is slow but we can integrate user code without changing it.
#define prints(id, fmt, ...)                                \
	do {                                                    \
		char buf[96];                                       \
		npf_snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); \
		triceS(id, "%s", buf);                              \
	} while (0)

//
//////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
