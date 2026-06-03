//! \file demoTILRpc.c
//! ///////////////////////////////////////////////////////////////////////////

//! Trice generated code - do not edit!

#include <stdio.h> // needed for __attribute__((weak)) 
#include "demoTILRpc.h"

//! triceRpc contains all rpc IDs together with their function pointer address.
const triceRpc_t triceRpc[] = {
	/* Trice type */  //  id, function pointer
	/*    Trice8F */ { 13298, FunctionNameWc },
	/*   Trice16F */ { 15298, FunctionNameXc },
	/*  TRICE64_F */ { 13308, FunctionNameZa },
	/*   TRice32F */ { 15301, FunctionNameYb },
	/*   Trice32F */ { 13306, FunctionNameYc },
	/*   trice16F */ { 13303, FunctionNameXd },
	/*   trice32F */ { 15303, FunctionNameYd },
	/*   TRICE8_F */ { 15292, FunctionNameWa },
	/*  TRICE64_F */ { 15304, FunctionNameZa },
	/*   TRice16F */ { 13301, FunctionNameXb },
	/*   TRice64F */ { 13309, FunctionNameZb },
	/*   trice16F */ { 15299, FunctionNameXd },
	/*   Trice64F */ { 15306, FunctionNameZc },
	/*   Trice64F */ { 13310, FunctionNameZc },
	/*   Trice16F */ { 13302, FunctionNameXc },
	/*  TRICE16_F */ { 13300, FunctionNameXa },
	/*  TRICE16_F */ { 15296, FunctionNameXa },
	/*   TRICE8_F */ { 15252, TryoutStructFunction },
	/*    Trice8F */ { 15294, FunctionNameWc },
	/*    TRice8F */ { 15293, FunctionNameWb },
	/*   TRice16F */ { 15297, FunctionNameXb },
	/*   TRice64F */ { 15305, FunctionNameZb },
	/*   trice64F */ { 13311, FunctionNameZd },
	/*   Trice32F */ { 15302, FunctionNameYc },
	/*    TRice8F */ { 13297, FunctionNameWb },
	/*   trice32F */ { 13307, FunctionNameYd },
	/*   TRICE8_F */ { 15253, TryoutBufferFunction },
	/*   Trice16F */ { 16337, ARemoteFunctionName },
	/*    trice8F */ { 13299, FunctionNameWd },
	/*   TRice32F */ { 13305, FunctionNameYb },
	/*   TRICE8_F */ { 13296, FunctionNameWa },
	/*    trice8F */ { 15295, FunctionNameWd },
	/*  TRICE32_F */ { 15300, FunctionNameYa },
	/*  TRICE32_F */ { 13304, FunctionNameYa },
	/*   trice64F */ { 15307, FunctionNameZd }
};

//! triceRpcListElements holds the compile time computed count of list elements.
const unsigned triceRpcElements = sizeof(triceRpc) / sizeof(triceRpc_t);

void FunctionNameZa( int64_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameWc( int8_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameXc( int16_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameZa( int64_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameYb( int32_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameYc( int32_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameXd( int16_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameYd( int32_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameWa( int8_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameXc( int16_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameXa( int16_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameXb( int16_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameZb( int64_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameXd( int16_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameZc( int64_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameZc( int64_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameZb( int64_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameZd( int64_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameYc( int32_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameXa( int16_t* p, int cnt) __attribute__((weak)) {}
void TryoutStructFunction( int8_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameWc( int8_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameWb( int8_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameXb( int16_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameYa( int32_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameZd( int64_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameWb( int8_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameYd( int32_t* p, int cnt) __attribute__((weak)) {}
void TryoutBufferFunction( int8_t* p, int cnt) __attribute__((weak)) {}
void ARemoteFunctionName( int16_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameWd( int8_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameYb( int32_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameWa( int8_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameWd( int8_t* p, int cnt) __attribute__((weak)) {}
void FunctionNameYa( int32_t* p, int cnt) __attribute__((weak)) {}
