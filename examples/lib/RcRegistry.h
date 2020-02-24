/*! \file RcRegistry.h
\brief 
\details
\todo lint code
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#ifndef RCCOMMANDERTOGATEREGISTRY_H_
#define RCCOMMANDERTOGATEREGISTRY_H_

#ifdef __cplusplus
extern "C" {
#endif


#ifdef AUTOMATIC_HEADER_INLUSION_
// use this for doxygen, define AUTOMATIC_HEADER_INLUSION_ in
// .doxygen file section PREDEFINED
//#include "config.h" // should be first include file, at least befor other project specific files
#include <stdint.h>
//#include <string.h>
//#include <stdarg.h>
//#include "CommonTypes.h"
#include "RcPack.h"
#endif // #ifdef AUTOMATIC_HEADER_INLUSION_


#define FUNCTIONTYPE( a ) { a ## _Client, a ## _Server, (void*)a, sizeof(a) / sizeof( void*) }



void PerformRemoteAppReset( void );
void PerformRemoteRblReset( void );

extern RcRegistry_t RcRegistry;


#ifdef __cplusplus
}
#endif

#endif /* RCCOMMANDERTOGATEREGISTRY_H_ */

