//! \file demoTILRpc.h
//! ///////////////////////////////////////////////////////////////////////////

//! Trice generated code - do not edit!

#include <stdint.h>

typedef void (*triceRpcHandler_t)(void* buffer, int count);

typedef struct{
    int id;
    triceRpcHandler_t fn;
} triceRpc_t;

extern triceRpc_t triceRpc[];
extern int triceRpcCount;

/*   TRice64F */ void FunctionNameZb( int64_t* p, int cnt );
/*   trice16F */ void FunctionNameXd( int16_t* p, int cnt );
/*   Trice64F */ void FunctionNameZc( int64_t* p, int cnt );
/*   Trice64F */ void FunctionNameZc( int64_t* p, int cnt );
/*   Trice16F */ void FunctionNameXc( int16_t* p, int cnt );
/*  TRICE16_F */ void FunctionNameXa( int16_t* p, int cnt );
/*   TRice16F */ void FunctionNameXb( int16_t* p, int cnt );
/*  TRICE16_F */ void FunctionNameXa( int16_t* p, int cnt );
/*   TRICE8_F */ void TryoutStructFunction( int8_t* p, int cnt );
/*    Trice8F */ void FunctionNameWc( int8_t* p, int cnt );
/*    TRice8F */ void FunctionNameWb( int8_t* p, int cnt );
/*   TRice16F */ void FunctionNameXb( int16_t* p, int cnt );
/*   TRice64F */ void FunctionNameZb( int64_t* p, int cnt );
/*   trice64F */ void FunctionNameZd( int64_t* p, int cnt );
/*   Trice32F */ void FunctionNameYc( int32_t* p, int cnt );
/*   trice32F */ void FunctionNameYd( int32_t* p, int cnt );
/*   TRICE8_F */ void TryoutBufferFunction( int8_t* p, int cnt );
/*   Trice16F */ void ARemoteFunctionName( int16_t* p, int cnt );
/*    trice8F */ void FunctionNameWd( int8_t* p, int cnt );
/*   TRice32F */ void FunctionNameYb( int32_t* p, int cnt );
/*   TRICE8_F */ void FunctionNameWa( int8_t* p, int cnt );
/*    trice8F */ void FunctionNameWd( int8_t* p, int cnt );
/*  TRICE32_F */ void FunctionNameYa( int32_t* p, int cnt );
/*  TRICE32_F */ void FunctionNameYa( int32_t* p, int cnt );
/*   trice64F */ void FunctionNameZd( int64_t* p, int cnt );
/*    TRice8F */ void FunctionNameWb( int8_t* p, int cnt );
/*   Trice16F */ void FunctionNameXc( int16_t* p, int cnt );
/*  TRICE64_F */ void FunctionNameZa( int64_t* p, int cnt );
/*   TRice32F */ void FunctionNameYb( int32_t* p, int cnt );
/*   Trice32F */ void FunctionNameYc( int32_t* p, int cnt );
/*   trice16F */ void FunctionNameXd( int16_t* p, int cnt );
/*   trice32F */ void FunctionNameYd( int32_t* p, int cnt );
/*   TRICE8_F */ void FunctionNameWa( int8_t* p, int cnt );
/*  TRICE64_F */ void FunctionNameZa( int64_t* p, int cnt );
/*    Trice8F */ void FunctionNameWc( int8_t* p, int cnt );

// End of file
