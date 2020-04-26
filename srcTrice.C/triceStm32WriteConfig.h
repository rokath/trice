/*! \file triceStm32WriteConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_STM32_WRITE_CONFIG_H_
#define TRICE_STM32_WRITE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "triceStm32CompilerConfig.h"
#include "main.h" // hardware specific stuff

///////////////////////////////////////////////////////////////////////////////
// hardware specific interface functions tested on NUCLEO-STM32F030
//

#define TRICE_USE_WRITE_FUNCTION // for a quick start you can enable this line and rely only putchar

#define TRICE_WRITE_OUT_FUNCTION STM32_LLDRV

void triceServeTransmit( void );

#ifdef __cplusplus
}
#endif

#endif /* TRICE_STM32_WRITE_CONFIG_H_ */
