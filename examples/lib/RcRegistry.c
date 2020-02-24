/*! \file RcRegistry.c
\brief remote call registry for remote function calls
\details
\author Thomas Hoehenleitner
*******************************************************************************/

#include "config.h" // should be first include file, at least befor other project specific files

#ifdef TRACELOG
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <CriticalSection.h>
#include "Tr16.h"
#include "TraceUtilities.h"
#include "Fifo.h"
#include "ReadAndWrite.h"
#endif // #ifdef TRACELOG

//todo go #include <stdio.h> // size_t
//todo go #include "Fifo.h"
#include "RcPack.h" // type handler
//todo go #include "Tr16.h"
//todo go #include "Trace2RemoteCall.h"
//#include "DaliTxEmergencyInterface.h" // DaliTxEmergencyCmdExpectingAnswer
//#include "DaliTxInterface.h" // DaliTxEmergencyCmdExpectingAnswer
//#include "RblXchange.h"
#include "RcRegistry.h"
//#include "Stb4Control.h"



#ifndef WINDOWS_OS
__weak int32_t DaliTxEmergencyCmdExpectingAnswer(uint8_t value_SHORT_ADDRESS, uint8_t opcodeByte, uint8_t* pResponse) { for(;;); /*return 0;*/ } //!< dummy \param value_SHORT_ADDRESS \param opcodeByte \param pResponse \return 0
__weak int32_t DaliTxStdCmdExpectingAnswer      (uint8_t value_SHORT_ADDRESS, uint8_t opcodeByte, uint8_t* pResponse) { for(;;); /*return 0;*/ } //!< dummy \param value_SHORT_ADDRESS \param opcodeByte \param pResponse \return 0
#endif // #ifndef WINDOWS_OS


/*! function registry for functions of type v_bboB_t 
placing type matching server functions here is enough to be able to call them from client side
*/
ALIGN4 static  w_bboB_t w_bboB[] ALIGN4_END = { //static int (*w_bboB[])( uint8_t, uint8_t, uint8_t* ) = {
    DaliTxEmergencyCmdExpectingAnswer, // tested
    DaliTxStdCmdExpectingAnswer,
};


#ifndef WINDOWS_OS
__weak int32_t DaliTxEmergencyCmdNoAnswer( uint8_t value_SHORT_ADDRESS, uint8_t opcodeByte ){ for(;;); /*return 0;*/ } //!< dummy \param value_SHORT_ADDRESS \param opcodeByte \return 0
__weak int32_t DaliTxStdCmdNoAnswer      ( uint8_t value_SHORT_ADDRESS, uint8_t opcodeByte ){ for(;;); /*return 0;*/ } //!< dummy \param value_SHORT_ADDRESS \param opcodeByte \return 0
__weak int32_t DaliTxSpecialCmdNoAnswer  ( uint8_t value_SHORT_ADDRESS, uint8_t opcodeByte ){ for(;;); /*return 0;*/ } //!< dummy \param value_SHORT_ADDRESS \param opcodeByte \return 0
__weak int32_t SetSwitchInputOnTestboard4( uint8_t select, uint8_t state )                  { for(;;); /*return 0;*/ } //!< dummy \param select \param state \return 0
#else

/*!
\param select channel
\param state 0 or 1
\return RESULT_OK or RESULT_ERROR
*/
int32_t SetSwitchInputOnTestboard4(uint8_t select, uint8_t state) {
	int r = 0; // result of called server function
	int result = Rc(DALIgate, RcCmd, SetSwitchInputOnTestboard4, &r, select, state);
	//ASSERT_OR_RETURN_RESULT(RESULT_OK == result, result);
	return r;
	//return RESULT_OK;
}
#endif // #ifndef WINDOWS_OS


/*! function registry for functions of type v_bb_t 
placing type matching server functions here is enough to be able to call them from client side
*/
ALIGN4 static  w_bb_t w_bb[] ALIGN4_END = {
    DaliTxEmergencyCmdNoAnswer,
    DaliTxStdCmdNoAnswer, // tested
    DaliTxSpecialCmdNoAnswer,
    SetSwitchInputOnTestboard4,
};


#ifndef WINDOWS_OS
__weak int32_t StoreEmergencySpecificValue( uint8_t ShortAddress, uint8_t opCode, uint8_t value ){ for(;;); /*return 0;*/ } //!< dummy \param ShortAddress \param opCode \param value \return 0
__weak int32_t StoreArcSpecificValue      ( uint8_t ShortAddress, uint8_t opCode, uint8_t value ){ for(;;); /*return 0;*/ } //!< dummy \param ShortAddress \param opCode \param value \return 0
#endif // #ifndef WINDOWS_OS


/*! function registry for functions of type v_bbb_t 
placing type matching server functions here is enough to be able to call them from client side
*/
ALIGN4 static w_bbb_t w_bbb[] ALIGN4_END = { //static int (*w_bbb[])( uint8_t, uint8_t, uint8_t ) = {
    StoreEmergencySpecificValue, // tested
    StoreArcSpecificValue,
};


#ifndef WINDOWS_OS
__weak void PerformRemoteAppReset( void ){ for(;;); } //!< dummy 
__weak void PerformRemoteRblReset( void ){ for(;;); } //!< dummy 
__weak void AllSwitchInputsTo0( void ){ for(;;); } //!< dummy 
__weak void AllSwitchInputsTo1( void ){ for(;;); } //!< dummy 
#else
void AllSwitchInputsTo0(void);
void AllSwitchInputsTo1(void);
#endif // #ifndef WINDOWS_OS


/*! function registry for functions of type v_v_t 
placing type matching server functions here is enough to be able to call them from client side
*/
ALIGN4 static v_v_t v_v[] ALIGN4_END = {
    PerformRemoteAppReset, // tested
    PerformRemoteRblReset, // tested
    AllSwitchInputsTo0,
    AllSwitchInputsTo1
};


#ifndef WINDOWS_OS
__weak int32_t RblReceive(uint8_t count, uint8_t* pBuff) { for(;;); /*return 0;*/  } //!< dummy \param count \param pBuff \return 0
__weak int32_t CurrentTime(uint8_t count, uint8_t* pBuff) { for(;;); /*return 0;*/  } //!< dummy \param count \param pBuff \return 0
__weak int32_t Stb4FifoDepth(uint8_t count, uint8_t* pBuff) { for(;;); /*return 0;*/  } //!< dummy \param count \param pBuff \return 0
__weak int32_t Stb4FifoRead(uint8_t count, uint8_t* pBuff) { for(;;); /*return 0;*/  } //!< dummy \param count \param pBuff \return 0
#else
int32_t Stb4FifoDepth(uint8_t count, uint8_t* pBuff);
int32_t Stb4FifoRead(uint8_t count, uint8_t* pBuff);
#endif // #ifndef WINDOWS_OS


/*! function registry for functions of type w_coC_t 
placing type matching server functions here is enough to be able to call them from client side
*/
ALIGN4 static w_coC_t w_coC[] ALIGN4_END = {
    RblReceive,
    CurrentTime,
    Stb4FifoDepth,
    Stb4FifoRead
};


#ifndef WINDOWS_OS
__weak int32_t RblSend( uint8_t count, const uint8_t* pBuff ){ for(;;); /*return 0;*/  } //!< dummy \param count \param pBuff \return 0
#endif // #ifndef WINDOWS_OS


/*! function registry for functions of type w_cC_t 
placing type matching server functions here is enough to be able to call them from client side
*/
ALIGN4 static w_cC_t w_cC[] ALIGN4_END = {
    RblSend,
};


#ifndef WINDOWS_OS
//! dummy  \param inCount \param inBuff \param outCount \param outBuff \return 0
__weak int32_t RblSendAndReceive( uint8_t inCount, const uint8_t* inBuff, uint8_t outCount, uint8_t* outBuff ){ for(;;); /*return 0;*/  }
#endif // #ifndef WINDOWS_OS


/*! function registry for functions of type w_coC_t 
placing type matching server functions here is enough to be able to call them from client side
*/
ALIGN4 static w_cCcoC_t w_cCcoC[] ALIGN4_END = {
    RblSendAndReceive,
};


/*! function registry for functions of type v_cC_t
placing type matching server functions here is enough to be able to call them from client side
*/
ALIGN4 static v_cC_t v_cC[] ALIGN4_END = {
    0,
};


#ifndef WINDOWS_OS
__weak void TraceLog_0 (uint8_t* pBuf){ for(;;); } //!< dummy \param pBuf
__weak void TraceLog_8 (uint8_t* pBuf){for(;;); }  //!< dummy \param pBuf
__weak void TraceLog_16(uint8_t* pBuf){ for(;;); } //!< dummy \param pBuf
__weak void TraceLog_32(uint8_t* pBuf){ for(;;); } //!< dummy \param pBuf
#endif // #ifndef WINDOWS_OS


/*! function registry for functions of type v_C_t
placing type matching server functions here is enough to be able to call them from client side
*/
ALIGN4 static v_C_t v_C[] ALIGN4_END = {
    0 /*TraceLog_0,
    TraceLog_8,
    TraceLog_16,
    TraceLog_32,*/
};


#ifndef WINDOWS_OS
__weak void SetAbg8tOutputs( uint8_t mask ){ for(;;); } //!< dummy \param mask
__weak void SendWaveCmdToSetDevice( uint8_t cmd ){ for(;;); } //!< dummy \param cmd
#else
void SetAbg8tOutputs(uint8_t mask);
void SendWaveCmdToSetDevice(uint8_t cmd);
#endif // #ifndef WINDOWS_OS


/*! function registry for functions of type v_c_t 
placing type matching server functions here is enough to be able to call them from client side
*/
ALIGN4 static v_c_t v_c[] = {
    SetAbg8tOutputs,
    SendWaveCmdToSetDevice,
};

//#if 0
///*! function registry for functions of type v_bB_t 
//placing type matching server functions here is enough to be able to call them from client side
//*/
//ALIGN4 static v_ccC8_t v_ccC8[] = {
//    TraceLog8, 
//};
//
//
///*! function registry for functions of type v_bB_t 
//placing type matching server functions here is enough to be able to call them from client side
//*/
//ALIGN4 static v_ccC16_t v_ccC16[] = {
//    TraceLog16, 
//};
//
//
///*! function registry for functions of type v_bB_t 
//placing type matching server functions here is enough to be able to call them from client side
//*/
//ALIGN4 static v_ccC32_t v_ccC32[] = {
//    TraceLog32, 
//};
//
//#endif // #if 0 

//! type registry
ALIGN4 static const Function_t FuncType[] ALIGN4_END = {
    FUNCTIONTYPE( v_C ),     // 0 - must be always on first position (tracelog!)
    FUNCTIONTYPE( v_cC ),    // 1
    FUNCTIONTYPE( w_cC ),    // 2
    FUNCTIONTYPE( w_coC ),   // 3
    FUNCTIONTYPE( w_cCcoC ), // 4
    FUNCTIONTYPE( w_bboB ),  // 5 tested
    FUNCTIONTYPE( w_bb ),    // 6 tested
    FUNCTIONTYPE( w_bbb ),   // 7 tested
    FUNCTIONTYPE( v_v ),     // 8 tested
    FUNCTIONTYPE( v_c ),     // 8
//#if 0
//    FUNCTIONTYPE( v_ccC8 ),  // 9
//    FUNCTIONTYPE( v_ccC16 ), // a
//    FUNCTIONTYPE( v_ccC32 ), // b
//#endif
};


 //! helper macro, computed at compile time
#define FuncTypeElements (sizeof(FuncType) / sizeof(Function_t)) // workaround


//! loaded on client and server
ALIGN4 RcRegistry_t RcRegistry ALIGN4_END = {
    FuncTypeElements,
    FuncType
};

