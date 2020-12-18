/*! \file trice.h
\author thomas.toehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_H_
#define TRICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "./intern/triceConfigCompiler.h"
#include "triceConfig.h"

#define SYNCED_BARE_ENCODING 4 //!< The trice bytes are sent in bare format with sync packages.
#define WRAPPED_BARE_ENCODING 8 //!< The trice bytes are sent in wrapped format.

#define Id(n) (n) //!< Macro for improved trice readability and better source code parsing.

#include "./intern/triceNoCode.h"
#include "./intern/triceEscFifo.h"
#include "./intern/triceBareFifo.h"

#ifdef __cplusplus
}
#endif

#endif // TRICE_H_
