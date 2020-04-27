/*! \file triceWrite.h
\author thomas.hoehenleitner [at] seerose.net
\brief 
\details 
*******************************************************************************/


#ifndef TRICEWRITE_H_
#define TRICEWRITE_H_

#ifdef __cplusplus
extern "C" {
#endif

int triceWrite( const void* p, int nbytes );
void triceWriteServer( void );

#ifdef __cplusplus
}
#endif

#endif /* TRICEWRITE_H_ */
