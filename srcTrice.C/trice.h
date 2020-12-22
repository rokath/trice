/*! \file trice.h
\author thomas.toehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_H_
#define TRICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_NOCODE_ENCODING 10
#define TRICE_FAST_BARE_SYNC_ENCODING 20
#define TRICE_LESS_BARE_SYNC_ENCODING 30
#define TRICE_FAST_BARE_WRAP_ENCODING 40
#define TRICE_LESS_BARE_WRAP_ENCODING 50
#define TRICE_FAST_BARE_L_SYNC_ENCODING 60
#define TRICE_LESS_BARE_L_SYNC_ENCODING 70
#define TRICE_FAST_BARE_L_WRAP_ENCODING 80
#define TRICE_LESS_BARE_L_WRAP_ENCODING 90
#define TRICE_ESC_ENCODING 100
#define TRICE_ESCL_ENCODING 200

#include "triceConfig.h"
#include "intern/triceConfigCompiler.h"
#include "intern/triceFifo.h"
#include "intern/triceSeggerRTT.h"

#include "intern/triceNoCode.h"
#include "intern/triceEsc.h"
#include "intern/triceFastBare.h"
//#include "intern/triceLessFlashBareL.h"

#include "intern/triceBareFifoToBytesBuffer.h"

#ifdef __cplusplus
}
#endif

#endif // TRICE_H_
