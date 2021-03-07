/*! \file triceNoCode.h
\author thomas.toehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_NO_CODE_H_
#define TRICE_NO_CODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TRICE_NO_CODE // no trice code generation //////////////////////////////

#define TRICE0( id, pFmt )
#define TRICE8_1( id, pFmt, v0                             )
#define TRICE8_2( id, pFmt, v0, v1                         )
#define TRICE8_3( id, pFmt, v0, v1, v2                     )
#define TRICE8_4( id, pFmt, v0, v1, v2, v3                 )
#define TRICE8_5( id, pFmt, v0, v1, v2, v3, v4             )
#define TRICE8_6( id, pFmt, v0, v1, v2, v3, v4, v5         )
#define TRICE8_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6     )
#define TRICE8_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7 )
#define TRICE16_1( id, pFmt, v0             )
#define TRICE16_2( id, pFmt, v0, v1         )
#define TRICE16_3( id, pFmt, v0, v1, v2     )
#define TRICE16_4( id, pFmt, v0, v1, v2, v3 )
#define TRICE32_1( id, pFmt, v0     )
#define TRICE32_2( id, pFmt, v0, v1 )
#define TRICE32_3( id, pFmt, v0, v1, v2 )
#define TRICE32_4( id, pFmt, v0, v1, v2, v3 )
#define TRICE64_1( id, pFmt, v0 )
#define TRICE64_2( id, pFmt, v0, v1 )

#endif // #ifdef TRICE_NO_CODE // no trice code generation ////////////////////

#ifdef __cplusplus
}
#endif

#endif // TRICE_NO_CODE_H_
