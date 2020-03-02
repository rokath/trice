/*! \file RcPack.h
\brief 
\details
\todo lint code
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#ifndef RCPACK_H_
#define RCPACK_H_

#ifdef __cplusplus
extern "C" {
#endif


#if 1 //def AUTOMATIC_HEADER_INLUSION_
// use this for doxygen, define AUTOMATIC_HEADER_INLUSION_ in
// .doxygen file section PREDEFINED
#include "config.h" // should be first include file, at least befor other project specific files
#include <stdint.h>
#include <stdarg.h>
#include "Fifo.h"
#endif // #ifdef AUTOMATIC_HEADER_INLUSION_

int greet(const char *name, int year, char *out);

void RcPackInit( size_t rdL, size_t wrL, size_t rdR, size_t wrR );
void RcRxServer( void );

typedef uint8_t rc_t; //!< remote call type descriptor

int Rc( uint8_t sad, rc_t t, const void* pFn, ... );

extern const rc_t RcCmd, RcCmdNoData, RcMsg, RcMsgNoData;


/*! remote call connection struct
*/
typedef struct tagRcRemote_t {
    uint8_t c0;  //!< fixed start byte
    uint8_t dpc; //!< following data package count: 0-255
    uint8_t pix; //!< packet index
    uint8_t cad; //!< local device address
    uint8_t sad; //!< destination device address (a broadcast for messages is possible)
    uint8_t tid; //!< type id
    uint8_t fid; //!< function id
    uint8_t crc8;//!< header checksum
} RcRemote_t; //lint !e761


typedef int32_t (*TypeHandler_t) (RcRemote_t*, va_list ); //!< wrapper function pointer signature


typedef struct {
    const TypeHandler_t ClientSide; //!< client side type handler for registered function
    const TypeHandler_t ServerSide; //!< server side type handler for registered function
    const void* const Array; //!< pointer to function pointer array
    unsigned const FunctionElements; //< count of registered functions of a special type 
} Function_t; //!< array element for remote call function registration

typedef struct {
    unsigned const TypeElements; //!< count of registerd function types
    const Function_t* const pFunctionType; //!< pointer to function type list
} RcRegistry_t; //!< fid and tid reference from a client address cad to a server address sad 


typedef void (*v_C_t)(uint8_t*); //!< byte count and byte buffer as input
int32_t v_C_Client(RcRemote_t* pReg, va_list argp); //!< client side function type handler
int32_t v_C_Server(RcRemote_t* rClient, va_list argp); //!< server side function type handler


typedef void (*v_cC_t)(uint8_t, uint8_t*); //!< byte count and byte buffer as input
int32_t v_cC_Client(RcRemote_t* pReg, va_list argp); //!< client side function type handler
int32_t v_cC_Server(RcRemote_t* rClient, va_list argp); //!< server side function type handler


typedef int32_t (*w_cC_t)( uint8_t, const uint8_t* ); //!< byte count and byte buffer as input
int32_t w_cC_Client( RcRemote_t* pReg, va_list argp ); //!< client side function type handler
int32_t w_cC_Server( RcRemote_t* rClient, va_list argp ); //!< server side function type handler


typedef int32_t (*w_coC_t)( uint8_t, uint8_t* ); //!< byte count and byte buffer as output
int32_t w_coC_Client( RcRemote_t* pReg, va_list argp ); //!< client side function type handler
int32_t w_coC_Server( RcRemote_t* rClient, va_list argp ); //!< server side function type handler


typedef int32_t (*w_cCcoC_t)( uint8_t, const uint8_t*, uint8_t, uint8_t* ); //!< byte count and byte buffer as input and byte count and buffer as output
int32_t w_cCcoC_Client( RcRemote_t* pReg, va_list argp ); //!< client side function type handler
int32_t w_cCcoC_Server( RcRemote_t* rClient, va_list argp ); //!< server side function type handler


typedef int32_t (*w_bboB_t)( uint8_t, uint8_t, uint8_t* ); //!< a byte as input, a max byte count as input a buffera dress where to store answer bytes
int32_t w_bboB_Client( RcRemote_t* pReg, va_list argp ); //!< client side function type handler
int32_t w_bboB_Server( RcRemote_t* t, va_list v); //!< server side function type handler


typedef int32_t (*w_bb_t)( uint8_t, uint8_t ); //!< 2 bytes as input
int32_t w_bb_Client( RcRemote_t* pReg, va_list argp ); //!< client side function type handler
int32_t w_bb_Server( RcRemote_t* t, va_list v); //!< server side function type handler


typedef int32_t (*w_bbb_t)( uint8_t, uint8_t, uint8_t ); //!< 3 bytes as input
int32_t w_bbb_Client( RcRemote_t* pReg, va_list argp ); //!< client side function type handler
int32_t w_bbb_Server( RcRemote_t* t, va_list v); //!< server side function type handler


typedef void (*v_v_t)( void ); //!< void void
int32_t v_v_Client( RcRemote_t* pReg, va_list argp ); //!< client side function type handler
int32_t v_v_Server( RcRemote_t* t, va_list v); //!< server side function type handler


typedef void (*v_c_t)( uint8_t ); //!< id as input
int v_c_Client( RcRemote_t* pReg, va_list argp ); //!< client side function type handler
int v_c_Server(RcRemote_t* rClient, va_list argp); //!< server side function type handler


//typedef void (*v_ccC8_t)( uint8_t, size_t, ... ); //!< id, byte count and uint8_t args buffer as input
//int v_ccC8_Client( RcRemote_t* pReg, va_list argp ); //!< client side function type handler
//int v_ccC8_Server(RcRemote_t* rClient, va_list argp); //!< server side function type handler
//
//
//typedef void (*v_ccC16_t)( uint8_t, size_t, ... ); //!< id, byte count and uint16_t args buffer as input
//int v_ccC16_Client( RcRemote_t* pReg, va_list argp ); //!< client side function type handler
//int v_ccC16_Server(RcRemote_t* rClient, va_list argp); //!< server side function type handler
//
//
//typedef void (*v_ccC32_t)( uint8_t, size_t, ... ); //!< id, byte count and uint32_t args buffer as input
//int v_ccC32_Client( RcRemote_t* pReg, va_list argp ); //!< client side function type handler
//int v_ccC32_Server(RcRemote_t* rClient, va_list argp); //!< server side function type handler


#ifdef __cplusplus
}
#endif

#endif /* RCPACK_H_ */

