/*! \file TriceUtilities.h
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
#include "Trice.h"
#endif // #ifdef AUTOMATIC_HEADER_INLUSION_



#if 0 < TRICE_LEVEL

#define REPORT_LINE(Value) do{ REPORT_FILE(); TRICE16_2( Id( 3340), " in line %d (0x%02x)\n", __LINE__, Value ); }while(0)
#define REPORT_FAILURE(Value) do{ REPORT_FILE(); TRICE32_2( Id(63699), "ERR: in line %d (0x%08x)\n", __LINE__, Value ); }while(0)
#define REPORT_FAILURE16(a,b,c) do{ REPORT_FILE(); TRICE16_4( Id(35139), "ERR: in line %d (0x%04x,0x%04x,0x%04x)\n", __LINE__, a,b,c ); }while(0)
#define REPORT_VALUE(Value)   do{ REPORT_FILE(); TRICE32_2( Id(16048),  "att: line %d, value = 0x%08x\r\n", __LINE__, Value ); }while(0)
#define REPORT_ONLY_VALUE(Value)   do{  TRICE32_2( Id(43582),  "att: line %d, value = 0x%08x\r\n", __LINE__, Value ); }while(0)

#define ERRMSG do{ REPORT_FILE(); TRICE16_1( Id(53007), "ERR: in line %d\n", __LINE__ ); }while(0)
#define ASSERT( flag ) do{ if(!(flag)) { ERRMSG; } }while(0) //!< report if flag is not true

#else // #if 0 < TRICE_LEVEL

#define REPORT_LINE(Value)
#define REPORT_FAILURE(Value) 
#define REPORT_FAILURE16(a,b,c)
#define REPORT_VALUE(Value)
#define ASSERTION
#define ASSERT( flag )

#endif // #else // #if 0 < TRICE_LEVEL

#define ASSERT_OR_RETURN( flag )                if(!(flag)) { ERRMSG; return;              } //!< if flag is not true return result
#define ASSERT_OR_RETURN_RESULT( flag, result ) if(!(flag)) { ERRMSG; return result;       } //!< if flag is not true return result
//#define ASSERT_OR_RETURN_RESULT_ERROR( flag )   if(!(flag)) { ERRMSG; return RESULT_ERROR; } //!< if flag is not true return result


void WaitForTraceLogTransmissionDone( void );


#ifdef __cplusplus
}
#endif

#endif /* TRICE_UTILITIES_H_ */
