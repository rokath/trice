// SPDX-License-Identifier: MIT
//! \file triceConfig.h

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_LEGACY_RPC_SUPPORT 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_AUXILIARY32 1
#define TRICE_CGO 1
#define TRICE_CYCLE_COUNTER 0

//////////////////////////////////////////////////////////////////////////////
// aliases - we have to exclude this in _setup_trice_environment.sh
//
#include "nanoprintf.h"

//! printi is a user print example with only integer values and therefore replacable direct with trice.
#define printi trice

//! prints is a user print example with string, float and integer values.
//! We have to perform a normal print into a buffer, which then is passed to triceS.
//! This is slow but we can integrate user code without changing it.
// Route the registered simple alias at its invocation site so bound sources
// obtain the ID from their sidecar and inserted sources retain their ID argument.
#define PRINTS_WITH_ID(id, ...)                        \
	do {                                               \
		char buf[96];                                  \
		npf_snprintf(buf, sizeof(buf), __VA_ARGS__);   \
		TRICE_INSERT_triceS(id, "%s", buf);            \
	} while (0)
#define prints(...) \
	TRICE_BIND_DISPATCH(TRICE_BIND_ROUTE(TRICE_BIND_FILE_KEY), PRINTS_WITH_ID, __VA_ARGS__)

//
//////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
