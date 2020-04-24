/*! \file triceConfig.h
\brief This file is for hardware/compiler specific project settings
\details 
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_ARDUINO_CONFIG_H_
#define TRICE_ARDUINO_CONFIG_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "Arduino.h" // hardware specific stuff

///////////////////////////////////////////////////////////////////////////////
// compiler adaptions
//

#define TRICE_INLINE static inline //! used for trice code

#define ALIGN4                                    //!< align to 4 byte boundary preamble
#define ALIGN4_END //__attribute__ ((aligned(4))) //!< align to 4 byte boundary post declaration
#define PACKED                                    //!< pack data preamble
#define PACKED_END //__attribute__ ((packed))      //!< pack data post declaration

///////////////////////////////////////////////////////////////////////////////
// hardware specific interface functions tested on NUCLEO-STM32F030
//

#define TRICE_WRITE_OUT_FUNCTION // for a quick start you can enable this line and rely only on a write

void triceServeTransmit( void );
int triceWrite( char* buf, int count );

#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }

#define SYSTICKVAL16 0 //!< STM32 specific, set to 0 as starting point with nonSTM MCE

#ifdef __cplusplus
}
#endif

#endif /* TRICE_ARDUINO_CONFIG_H_ */
