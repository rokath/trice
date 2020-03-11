/*! \file Fifo.h
\brief 
\details
\todo lint code
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#ifndef Fifo_H_
#define Fifo_H_ //lint !e750

#ifdef __cplusplus
extern "C" {
#endif


#if 1 //def AUTOMATIC_HEADER_INLUSION_
// use this for doxygen, define AUTOMATIC_HEADER_INLUSION_ in
// .doxygen file section PREDEFINED
#include "config.h" // should be first include file, at least befor other project specific files
#include <stdint.h>
#include <stdio.h> // size_t
//lint -e2701 -e528 -e752 -e754
#endif // #ifdef AUTOMATIC_HEADER_INLUSION_


typedef struct{
    size_t size; //!< fifo data size
    uint8_t* pBuff; //!< start address
    uint8_t* pLimit; //!< first address behind fifo data
    uint8_t* pWr; //!< internal write pointer
    uint8_t* pRd; //!< internal read pointer
} Fifo_t; //!< first in first out struct type


void FifoInit( Fifo_t* f, size_t size );

#if 1

size_t FifoWritableCount_Protected( Fifo_t const * const f );
size_t FifoReadableCount_Protected( Fifo_t const * const f );

size_t FifoWritableCount_Unprotected( Fifo_t const * const f );
size_t FifoReadableCount_Unprotected( Fifo_t const * const f );

#else

/*!
\param f pointer to fifo struct
\param count a computed count (could be negative)
\return count if count was positive or with size corrected value as correct count
*/
static inline size_t adjustCount( Fifo_t const * const f, int count ){
    count = count < 0 ? count + (unsigned)(f->size) : count; //lint !e713 !e737
    return (size_t)count;
}


/*! fifo depth
\details a fifo of size n (n>1!) can hold only n-1 values to be able to distinct between empty and full
\param f pointer to fifo struct
\retval Count of bytes actually in fifo
*/
static inline size_t FifoReadableCount( Fifo_t const * const f ){
    return adjustCount( f, (int)(f->pWr - f->pRd) );
}


/*! space left in fifo
\details a fifo of size n (n>1!) can hold only n-1 values to be able to distinct between empty and full
\param f pointer to fifo struct
\retval Count of bytes actually can be written into fifo
*/
static inline size_t FifoWritableCount( Fifo_t const * const f ){
    return f->size - FifoReadableCount( f ) - 1; 
}

#endif

//int FifoWritableBlockSpace( Fifo_t* f );
//int FifoReadableBlockSpace( Fifo_t* f );

void FifoPushUint8_Unprotected( Fifo_t* f, uint8_t value );
void FifoPopUint8_Unprotected( Fifo_t* f, uint8_t* pValue );
void FifoPushUint32_Unprotected( Fifo_t* f, uint32_t v );
void FifoPushBuffer_Unprotected( Fifo_t* f, size_t count, const uint8_t* pBuff );
void FifoPopBuffer_Unprotected( Fifo_t* f, size_t count, uint8_t* pBuff );

void FifoPushUint8_Protected( Fifo_t* f, uint8_t value );
void FifoPopUint8_Protected( Fifo_t* f, uint8_t* pValue );
void FifoPushUint32_Protected( Fifo_t* f, uint32_t v );
void FifoPushBuffer_Protected( Fifo_t* f, size_t count, const uint8_t* pBuff );
void FifoPopBuffer_Protected( Fifo_t* f, size_t count, uint8_t* pBuff );


//int FifoWaitForWritableCount(Fifo_t const * const f, size_t count );
//int FifoWaitForReadableCount(Fifo_t const* const f, size_t count );
//int FifoWaitForReadableCount(Fifo_t const* const f, size_t count, unsigned ms );


//! return amount of bytes continuously can be written without write pointer wrap
//! This is __not__ the free writable count! See also FifoWritableCount().
//! \param f pointer to fifo struct
//! \retval size of bytes which can be continuously written without write pointer wrap
static inline size_t FifoWritableBlockSpace( const Fifo_t* f ){
    return (size_t)(f->pLimit - f->pWr);
}


/*! return amount of bytes continuously can be read without read pointer wrap
This is __not__ the readable amount! See also FifoReadableCount().
\param f pointer to fifo struct
\retval size of bytes which can be continuously read without read pointer wrap
*/
static inline size_t FifoReadableBlockSpace( const Fifo_t* f ){
    return (size_t)(f->pLimit - f->pRd);
}

//int FifoFlushBlocked( Fifo_t* f, int pFn( size_t, uint8_t* ) );
//void FifoTryFlushUnBlocked( Fifo_t* f, size_t pFn( size_t, uint8_t*) );

#ifdef __cplusplus
}
#endif

#endif /* Fifo_H_ */

