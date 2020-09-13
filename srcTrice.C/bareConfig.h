/*! \file bareConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef BARE_CONFIG_H_
#define BARE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// main settings
//

#ifndef TRICE_PORT
//! TRICE_PORT defines the trice output port: RTT or UART.
#define TRICE_PORT RTT
#endif

#ifndef TRICE_CODE
//! Set to MORE_FLASH_AND_SPEED, LESS_FLASH_AND_SPEED or NO_CODE
#define TRICE_CODE MORE_FLASH_AND_SPEED
#endif

//
///////////////////////////////////////////////////////////////////////////////

#if RTT == TRICE_PORT
#include "bareConfigRTT.h"
#elif UART == TRICE_PORT
#include "bareConfigUART.h"
#else
#error
#endif

//!< tests
void triceCheckSet( int index ); 

#ifdef __cplusplus
}
#endif

#endif /* BARE_CONFIG_H_ */
