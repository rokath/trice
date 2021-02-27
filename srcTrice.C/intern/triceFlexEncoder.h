/*! \file tricePack2Encoder.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_FLEX_ENCODER_H_
#define TRICE_FLEX_ENCODER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define id(n) ((((uint32_t)(n))<<(32-16)) & ~0x80000000) //!< Prepare 15-bit ID for transmission, bit 31 is 0
#define Id(n) ((((uint32_t)(n))<<(32-21)) |  0x80000000) //!< Prepare 20-bit ID for transmission, bit 31 is 1

#include "tricePack2Encoder.h"

//! Trice0 does trace id protected inside critical section in short mode.
//! \param id trice identifier
//! \param pFmt formatstring for trice
#define Trice0i( id, pFmt ) do{ \
    TRICE_HTON_U32PUSH( id ); \
} while(0)


//! Trice0 does trace id protected outside critical section in short mode.
//! \param id trice identifier
//! \param pFmt formatstring for trice
#define Trice0( id, pFmt ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    Trice0i( id, pFmt ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

//! Trice16_1i does trace id and 16-bit value protected inside critical section in short mode.
//! The passed d0 must not exeed the 16-bit size.
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define Trice16_1i( id, pFmt, d0 ) do{ \
    TRICE_HTON_U32PUSH( id | /*(uint16_t)*/d0 ); \
} while(0)

//! Trice16_1 does trace id and 16-bit value protected (outside critical section) in short mode.
//! The passed d0 must not exeed the 16-bit size.
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
#define Trice16_1( id, pFmt, d0 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    Trice16_1i( id, pFmt, d0 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

#define Trice8_1i Trice16_1i // same code
#define Trice8_1  Trice16_1 // same code


//! Trice8_2i does trace id and 8-bit values protected (outside critical section) in short mode.
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! This is a time optinized implementation.
#define Trice8_2i( id, pFmt, d0, d1 ) do{ \
    TRICE_HTON_U32PUSH( id|TRICE_U8_JOIN(d0,d1) ); \
} while(0)

//! Trice8_2 does trace id and 8-bit values protected (outside critical section) in short mode.
//! \param id trice identifier
//! \param pFmt formatstring for trice
//! \param d0 payload
//! \param d1 payload
//! This is a time optinized implementation.
#define Trice8_2( id, pFmt, d0, d1 ) do{ \
    TRICE_ENTER_CRITICAL_SECTION \
    Trice8_2i( id, pFmt, d0, d1 ); \
    TRICE_LEAVE_CRITICAL_SECTION \
} while(0)

#ifdef __cplusplus
}
#endif

#endif /* TRICE_FLEX_ENCODER_H_ */
