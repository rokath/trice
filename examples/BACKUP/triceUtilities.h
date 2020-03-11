/*! \file triceUtilities.h
\brief 
\details 
\author Thomas Hoehenleitner
*******************************************************************************/

#ifndef TRICE_UTILITIES_H_
#define TRICE_UTILITIES_H_

#ifdef __cplusplus
extern "C" {
#endif


#if 1 //def AUTOMATIC_HEADER_INLUSION_
// use this for doxygen, define AUTOMATIC_HEADER_INLUSION_ in
// .doxygen file section PREDEFINED
#include "config.h" // should be first include file, at least befor other project specific files
#include <stdint.h>
//#include "DefaultMacros.h"
#include "trice.h"
#endif // #ifdef AUTOMATIC_HEADER_INLUSION_


void WaitForTraceLogTransmissionDone( void );


#ifdef __cplusplus
}
#endif

#endif /* TRICE_UTILITIES_H_ */
