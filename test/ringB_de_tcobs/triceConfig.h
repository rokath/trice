/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_SINGLE_MAX_SIZE 400
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_BUFFER_SIZE 4096

#define TRICE_CGO 1
#define TRICE_CYCLE_COUNTER 0


#include "triceDefaultConfig.h"

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
