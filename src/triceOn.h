/*! \file triceOn.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#include "triceVariadic.h"

// clang-format off
#define TRICE(  tid, fmt, ...) TRICE_CONCAT2(TRICE_,   TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define trice(  tid, fmt, ...) TRICE_CONCAT2(trice_,   TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define Trice(  tid, fmt, ...) TRICE_CONCAT2(Trice_,   TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define TRice(  tid, fmt, ...) TRICE_CONCAT2(TRice_,   TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define TRICE8( tid, fmt, ...) TRICE_CONCAT2(TRICE8_,  TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define trice8( tid, fmt, ...) TRICE_CONCAT2(trice8_,  TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define Trice8( tid, fmt, ...) TRICE_CONCAT2(Trice8_,  TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define TRice8( tid, fmt, ...) TRICE_CONCAT2(TRice8_,  TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define TRICE16(tid, fmt, ...) TRICE_CONCAT2(TRICE16_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define trice16(tid, fmt, ...) TRICE_CONCAT2(trice16_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define Trice16(tid, fmt, ...) TRICE_CONCAT2(Trice16_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define TRice16(tid, fmt, ...) TRICE_CONCAT2(TRice16_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define TRICE32(tid, fmt, ...) TRICE_CONCAT2(TRICE32_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define trice32(tid, fmt, ...) TRICE_CONCAT2(trice32_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define Trice32(tid, fmt, ...) TRICE_CONCAT2(Trice32_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define TRice32(tid, fmt, ...) TRICE_CONCAT2(TRice32_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define TRICE64(tid, fmt, ...) TRICE_CONCAT2(TRICE64_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define trice64(tid, fmt, ...) TRICE_CONCAT2(trice64_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define Trice64(tid, fmt, ...) TRICE_CONCAT2(Trice64_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define TRice64(tid, fmt, ...) TRICE_CONCAT2(TRice64_, TRICE_COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
// clang-format on

//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// global function prototypes: (not all always used)

void TriceInitRingBufferMargins(void);
void WatchRingBufferMargins(void);
void TriceCheck(int index); //!< tests and examples
void TriceDiagnostics(int index);
void TriceNonBlockingWriteUartA(const void* buf, size_t nByte);
void TriceNonBlockingWriteUartB(const void* buf, size_t nByte);
void TriceNonBlockingDirectWrite(uint32_t* triceStart, unsigned wordCount);
void TriceNonBlockingDirectWrite8Auxiliary(const uint8_t* enc, size_t encLen);
void TriceNonBlockingDeferredWrite8Auxiliary(const uint8_t* enc, size_t encLen);
void TriceNonBlockingDirectWrite32Auxiliary(const uint32_t* enc, unsigned count);
//void TriceNonBlockingDeferredWrite32Auxiliary(const uint32_t* enc, unsigned count);
void TriceInit(void);
void TriceLogDiagnosticData(void);
void TriceLogSeggerDiagnostics(void);
void TriceNonBlockingDeferredWrite8(int ticeID, const uint8_t* enc, size_t encLen);
void TriceTransfer(void);
size_t triceDataLen(const uint8_t* p);
int TriceEnoughSpace(void);
unsigned TriceOutDepth(void);
size_t TriceDepth(void);
size_t TriceDepthMax(void);
size_t TriceEncode(unsigned encrypt, unsigned framing, uint8_t* dst, const uint8_t* buf, size_t len);
void TriceWriteDeviceCgo(const void* buf, unsigned len); //!< TriceWriteDeviceCgo is only needed for testing C-sources from Go.
unsigned TriceOutDepthCGO(void);                         //!< TriceOutDepthCGO is only needed for testing C-sources from Go.

void TriceBlockingWriteUartA(const uint8_t* buf, unsigned len);
void triceServeTransmitUartA(void);
void triceTriggerTransmitUartA(void);
unsigned TriceOutDepthUartA(void);

void TriceBlockingWriteUartB(const uint8_t* buf, unsigned len);
void triceServeTransmitUartB(void);
void triceTriggerTransmitUartB(void);
unsigned TriceOutDepthUartB(void);

void XTEAEncrypt(uint32_t* p, unsigned count);
void XTEADecrypt(uint32_t* p, unsigned count);
void XTEAInitTable(void);

// Trice functions and macros

void trice8fn_0(uint16_t tid);
void trice8fn_1(uint16_t tid, uint8_t v0);
void trice8fn_2(uint16_t tid, uint8_t v0, uint8_t v1);
void trice8fn_3(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2);
void trice8fn_4(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3);
void trice8fn_5(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4);
void trice8fn_6(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5);
void trice8fn_7(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6);
void trice8fn_8(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7);
void trice8fn_9(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8);
void trice8fn_10(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9);
void trice8fn_11(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10);
void trice8fn_12(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10, uint8_t v11);

void Trice8fn_0(uint16_t tid);
void Trice8fn_1(uint16_t tid, uint8_t v0);
void Trice8fn_2(uint16_t tid, uint8_t v0, uint8_t v1);
void Trice8fn_3(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2);
void Trice8fn_4(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3);
void Trice8fn_5(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4);
void Trice8fn_6(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5);
void Trice8fn_7(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6);
void Trice8fn_8(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7);
void Trice8fn_9(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8);
void Trice8fn_10(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9);
void Trice8fn_11(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10);
void Trice8fn_12(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10, uint8_t v11);

void TRice8fn_0(uint16_t tid);
void TRice8fn_1(uint16_t tid, uint8_t v0);
void TRice8fn_2(uint16_t tid, uint8_t v0, uint8_t v1);
void TRice8fn_3(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2);
void TRice8fn_4(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3);
void TRice8fn_5(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4);
void TRice8fn_6(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5);
void TRice8fn_7(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6);
void TRice8fn_8(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7);
void TRice8fn_9(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8);
void TRice8fn_10(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9);
void TRice8fn_11(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10);
void TRice8fn_12(uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10, uint8_t v11);

void trice16fn_0(uint16_t tid);
void trice16fn_1(uint16_t tid, uint16_t v0);
void trice16fn_2(uint16_t tid, uint16_t v0, uint16_t v1);
void trice16fn_3(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2);
void trice16fn_4(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3);
void trice16fn_5(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4);
void trice16fn_6(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5);
void trice16fn_7(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6);
void trice16fn_8(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7);
void trice16fn_9(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8);
void trice16fn_10(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9);
void trice16fn_11(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10);
void trice16fn_12(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10, uint16_t v11);

void Trice16fn_0(uint16_t tid);
void Trice16fn_1(uint16_t tid, uint16_t v0);
void Trice16fn_2(uint16_t tid, uint16_t v0, uint16_t v1);
void Trice16fn_3(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2);
void Trice16fn_4(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3);
void Trice16fn_5(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4);
void Trice16fn_6(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5);
void Trice16fn_7(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6);
void Trice16fn_8(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7);
void Trice16fn_9(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8);
void Trice16fn_10(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9);
void Trice16fn_11(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10);
void Trice16fn_12(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10, uint16_t v11);

void TRice16fn_0(uint16_t tid);
void TRice16fn_1(uint16_t tid, uint16_t v0);
void TRice16fn_2(uint16_t tid, uint16_t v0, uint16_t v1);
void TRice16fn_3(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2);
void TRice16fn_4(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3);
void TRice16fn_5(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4);
void TRice16fn_6(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5);
void TRice16fn_7(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6);
void TRice16fn_8(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7);
void TRice16fn_9(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8);
void TRice16fn_10(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9);
void TRice16fn_11(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10);
void TRice16fn_12(uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10, uint16_t v11);

void trice32fn_0(uint16_t tid);
void trice32fn_1(uint16_t tid, uint32_t v0);
void trice32fn_2(uint16_t tid, uint32_t v0, uint32_t v1);
void trice32fn_3(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2);
void trice32fn_4(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3);
void trice32fn_5(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4);
void trice32fn_6(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5);
void trice32fn_7(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6);
void trice32fn_8(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7);
void trice32fn_9(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8);
void trice32fn_10(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9);
void trice32fn_11(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10);
void trice32fn_12(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10, uint32_t v11);

void Trice32fn_0(uint16_t tid);
void Trice32fn_1(uint16_t tid, uint32_t v0);
void Trice32fn_2(uint16_t tid, uint32_t v0, uint32_t v1);
void Trice32fn_3(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2);
void Trice32fn_4(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3);
void Trice32fn_5(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4);
void Trice32fn_6(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5);
void Trice32fn_7(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6);
void Trice32fn_8(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7);
void Trice32fn_9(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8);
void Trice32fn_10(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9);
void Trice32fn_11(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10);
void Trice32fn_12(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10, uint32_t v11);

void TRice32fn_0(uint16_t tid);
void TRice32fn_1(uint16_t tid, uint32_t v0);
void TRice32fn_2(uint16_t tid, uint32_t v0, uint32_t v1);
void TRice32fn_3(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2);
void TRice32fn_4(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3);
void TRice32fn_5(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4);
void TRice32fn_6(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5);
void TRice32fn_7(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6);
void TRice32fn_8(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7);
void TRice32fn_9(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8);
void TRice32fn_10(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9);
void TRice32fn_11(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10);
void TRice32fn_12(uint16_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10, uint32_t v11);

void trice64fn_0(uint16_t tid);
void trice64fn_1(uint16_t tid, uint64_t v0);
void trice64fn_2(uint16_t tid, uint64_t v0, uint64_t v1);
void trice64fn_3(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2);
void trice64fn_4(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3);
void trice64fn_5(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4);
void trice64fn_6(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5);
void trice64fn_7(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6);
void trice64fn_8(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7);
void trice64fn_9(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8);
void trice64fn_10(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9);
void trice64fn_11(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10);
void trice64fn_12(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10, uint64_t v11);

void Trice64fn_0(uint16_t tid);
void Trice64fn_1(uint16_t tid, uint64_t v0);
void Trice64fn_2(uint16_t tid, uint64_t v0, uint64_t v1);
void Trice64fn_3(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2);
void Trice64fn_4(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3);
void Trice64fn_5(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4);
void Trice64fn_6(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5);
void Trice64fn_7(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6);
void Trice64fn_8(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7);
void Trice64fn_9(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8);
void Trice64fn_10(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9);
void Trice64fn_11(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10);
void Trice64fn_12(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10, uint64_t v11);

void TRice64fn_0(uint16_t tid);
void TRice64fn_1(uint16_t tid, uint64_t v0);
void TRice64fn_2(uint16_t tid, uint64_t v0, uint64_t v1);
void TRice64fn_3(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2);
void TRice64fn_4(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3);
void TRice64fn_5(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4);
void TRice64fn_6(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5);
void TRice64fn_7(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6);
void TRice64fn_8(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7);
void TRice64fn_9(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8);
void TRice64fn_10(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9);
void TRice64fn_11(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10);
void TRice64fn_12(uint16_t tid, uint64_t v0, uint64_t v1, uint64_t v2, uint64_t v3, uint64_t v4, uint64_t v5, uint64_t v6, uint64_t v7, uint64_t v8, uint64_t v9, uint64_t v10, uint64_t v11);

#define trice8_0(tid, fmt) trice8fn_0(tid)                                                                                                                                                                                                                                           //!< trice8_0 is a macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_1(tid, fmt, v0) trice8fn_1(tid, (uint8_t)(v0))                                                                                                                                                                                                                        //!< trice8_1 is a macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_2(tid, fmt, v0, v1) trice8fn_2(tid, (uint8_t)(v0), (uint8_t)(v1))                                                                                                                                                                                                     //!< trice8_2 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_3(tid, fmt, v0, v1, v2) trice8fn_3(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2))                                                                                                                                                                                  //!< trice8_3 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_4(tid, fmt, v0, v1, v2, v3) trice8fn_4(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3))                                                                                                                                                               //!< trice8_4 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_5(tid, fmt, v0, v1, v2, v3, v4) trice8fn_5(tid, (uint8_t)v0, (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4))                                                                                                                                              //!< trice8_5 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_6(tid, fmt, v0, v1, v2, v3, v4, v5) trice8fn_6(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5))                                                                                                                         //!< trice8_6 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) trice8fn_7(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6))                                                                                                      //!< trice8_7 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) trice8fn_8(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7))                                                                                   //!< trice8_8 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) trice8fn_9(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8))                                                                //!< trice8_9 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) trice8fn_10(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9))                                           //!< trice8_10 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) trice8fn_11(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9), (uint8_t)(v10))                      //!< trice8_1M ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) trice8fn_12(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9), (uint8_t)(v10), (uint8_t)(v11)) //!< trice8_12 ia macro calling a function to reduce code size, this way avoiding code inlining.

#define Trice8_0(tid, fmt) Trice8fn_0(tid)                                                                                                                                                                                                                                           //!< Trice8_1 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_1(tid, fmt, v0) Trice8fn_1(tid, (uint8_t)(v0))                                                                                                                                                                                                                        //!< Trice8_1 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_2(tid, fmt, v0, v1) Trice8fn_2(tid, (uint8_t)(v0), (uint8_t)(v1))                                                                                                                                                                                                     //!< Trice8_2 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_3(tid, fmt, v0, v1, v2) Trice8fn_3(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2))                                                                                                                                                                                  //!< Trice8_3 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_4(tid, fmt, v0, v1, v2, v3) Trice8fn_4(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3))                                                                                                                                                               //!< Trice8_4 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_5(tid, fmt, v0, v1, v2, v3, v4) Trice8fn_5(tid, (uint8_t)v0, (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4))                                                                                                                                              //!< Trice8_5 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_6(tid, fmt, v0, v1, v2, v3, v4, v5) Trice8fn_6(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5))                                                                                                                         //!< Trice8_6 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) Trice8fn_7(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6))                                                                                                      //!< Trice8_7 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) Trice8fn_8(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7))                                                                                   //!< Trice8_8 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) Trice8fn_9(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8))                                                                //!< Trice8_9 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) Trice8fn_10(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9))                                           //!< Trice8_10 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) Trice8fn_11(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9), (uint8_t)(v10))                      //!< Trice8_11 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) Trice8fn_12(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9), (uint8_t)(v10), (uint8_t)(v11)) //!< Trice8_12 ia macro calling a function to reduce code size, this way avoiding code inlining.

#define TRice8_0(tid, fmt) TRice8fn_0(tid)                                                                                                                                                                                                                                           //!< TRice8_0 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_1(tid, fmt, v0) TRice8fn_1(tid, (uint8_t)(v0))                                                                                                                                                                                                                        //!< TRice8_1 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_2(tid, fmt, v0, v1) TRice8fn_2(tid, (uint8_t)(v0), (uint8_t)(v1))                                                                                                                                                                                                     //!< TRice8_2 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_3(tid, fmt, v0, v1, v2) TRice8fn_3(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2))                                                                                                                                                                                  //!< TRice8_3 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_4(tid, fmt, v0, v1, v2, v3) TRice8fn_4(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3))                                                                                                                                                               //!< TRice8_4 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_5(tid, fmt, v0, v1, v2, v3, v4) TRice8fn_5(tid, (uint8_t)v0, (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4))                                                                                                                                              //!< TRice8_5 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_6(tid, fmt, v0, v1, v2, v3, v4, v5) TRice8fn_6(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5))                                                                                                                         //!< TRice8_6 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) TRice8fn_7(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6))                                                                                                      //!< TRice8_7 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) TRice8fn_8(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7))                                                                                   //!< TRice8_8 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) TRice8fn_9(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8))                                                                //!< TRice8_9 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) TRice8fn_10(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9))                                           //!< TRice8_10 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) TRice8fn_11(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9), (uint8_t)(v10))                      //!< TRice8_11 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) TRice8fn_12(tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9), (uint8_t)(v10), (uint8_t)(v11)) //!< TRice8_12 ia macro calling a function to reduce code size, this way avoiding code inlining.

#define trice16_0(tid, fmt) trice16fn_0(tid)                                                                                                                                                                                                                                                       //!< trice16_0 is a macro calling a function to reduce code size.
#define trice16_1(tid, fmt, v0) trice16fn_1(tid, (uint16_t)(v0))                                                                                                                                                                                                                                   //!< trice16_1 is a macro calling a function to reduce code size.
#define trice16_2(tid, fmt, v0, v1) trice16fn_2(tid, (uint16_t)(v0), (uint16_t)(v1))                                                                                                                                                                                                               //!< trice16_2 is a macro calling a function to reduce code size.
#define trice16_3(tid, fmt, v0, v1, v2) trice16fn_3(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2))                                                                                                                                                                                           //!< trice16_3 is a macro calling a function to reduce code size.
#define trice16_4(tid, fmt, v0, v1, v2, v3) trice16fn_4(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3))                                                                                                                                                                       //!< trice16_4 is a macro calling a function to reduce code size.
#define trice16_5(tid, fmt, v0, v1, v2, v3, v4) trice16fn_5(tid, (uint16_t)v0, (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4))                                                                                                                                                     //!< trice16_5 is a macro calling a function to reduce code size.
#define trice16_6(tid, fmt, v0, v1, v2, v3, v4, v5) trice16fn_6(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5))                                                                                                                               //!< trice16_6 is a macro calling a function to reduce code size.
#define trice16_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) trice16fn_7(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6))                                                                                                           //!< trice16_7 is a macro calling a function to reduce code size.
#define trice16_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) trice16fn_8(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7))                                                                                       //!< trice16_8 is a macro calling a function to reduce code size.
#define trice16_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) trice16fn_9(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8))                                                                   //!< trice16_9 is a macro calling a function to reduce code size.
#define trice16_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) trice16fn_10(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9))                                             //!< trice16_10 is a macro calling a function to reduce code size.
#define trice16_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) trice16fn_11(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10))                       //!< trice16_11 is a macro calling a function to reduce code size.
#define trice16_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) trice16fn_12(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10), (uint16_t)(v11)) //!< trice16_12 is a macro calling a function to reduce code size.

#define Trice16_0(tid, fmt) Trice16fn_0(tid)                                                                                                                                                                                                                                                       //!< Trice16_0 is a macro calling a function to reduce code size.
#define Trice16_1(tid, fmt, v0) Trice16fn_1(tid, (uint16_t)(v0))                                                                                                                                                                                                                                   //!< Trice16_1 is a macro calling a function to reduce code size.
#define Trice16_2(tid, fmt, v0, v1) Trice16fn_2(tid, (uint16_t)(v0), (uint16_t)(v1))                                                                                                                                                                                                               //!< Trice16_2 is a macro calling a function to reduce code size.
#define Trice16_3(tid, fmt, v0, v1, v2) Trice16fn_3(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2))                                                                                                                                                                                           //!< Trice16_3 is a macro calling a function to reduce code size.
#define Trice16_4(tid, fmt, v0, v1, v2, v3) Trice16fn_4(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3))                                                                                                                                                                       //!< Trice16_4 is a macro calling a function to reduce code size.
#define Trice16_5(tid, fmt, v0, v1, v2, v3, v4) Trice16fn_5(tid, (uint16_t)v0, (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4))                                                                                                                                                     //!< Trice16_5 is a macro calling a function to reduce code size.
#define Trice16_6(tid, fmt, v0, v1, v2, v3, v4, v5) Trice16fn_6(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5))                                                                                                                               //!< Trice16_6 is a macro calling a function to reduce code size.
#define Trice16_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) Trice16fn_7(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6))                                                                                                           //!< Trice16_7 is a macro calling a function to reduce code size.
#define Trice16_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) Trice16fn_8(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7))                                                                                       //!< Trice16_8 is a macro calling a function to reduce code size.
#define Trice16_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) Trice16fn_9(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8))                                                                   //!< Trice16_9 is a macro calling a function to reduce code size.
#define Trice16_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) Trice16fn_10(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9))                                             //!< Trice16_10 is a macro calling a function to reduce code size.
#define Trice16_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) Trice16fn_11(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10))                       //!< Trice16_11 is a macro calling a function to reduce code size.
#define Trice16_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) Trice16fn_12(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10), (uint16_t)(v11)) //!< Trice16_12 is a macro calling a function to reduce code size.

#define TRice16_0(tid, fmt) TRice16fn_0(tid)                                                                                                                                                                                                                                                       //!< TRice16_0 is a macro calling a function to reduce code size.
#define TRice16_1(tid, fmt, v0) TRice16fn_1(tid, (uint16_t)(v0))                                                                                                                                                                                                                                   //!< TRice16_1 is a macro calling a function to reduce code size.
#define TRice16_2(tid, fmt, v0, v1) TRice16fn_2(tid, (uint16_t)(v0), (uint16_t)(v1))                                                                                                                                                                                                               //!< TRice16_2 is a macro calling a function to reduce code size.
#define TRice16_3(tid, fmt, v0, v1, v2) TRice16fn_3(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2))                                                                                                                                                                                           //!< TRice16_3 is a macro calling a function to reduce code size.
#define TRice16_4(tid, fmt, v0, v1, v2, v3) TRice16fn_4(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3))                                                                                                                                                                       //!< TRice16_4 is a macro calling a function to reduce code size.
#define TRice16_5(tid, fmt, v0, v1, v2, v3, v4) TRice16fn_5(tid, (uint16_t)v0, (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4))                                                                                                                                                     //!< TRice16_5 is a macro calling a function to reduce code size.
#define TRice16_6(tid, fmt, v0, v1, v2, v3, v4, v5) TRice16fn_6(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5))                                                                                                                               //!< TRice16_6 is a macro calling a function to reduce code size.
#define TRice16_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) TRice16fn_7(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6))                                                                                                           //!< TRice16_7 is a macro calling a function to reduce code size.
#define TRice16_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) TRice16fn_8(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7))                                                                                       //!< TRice16_16_M is a macro calling a function to reduce code size.
#define TRice16_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) TRice16fn_9(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8))                                                                   //!< TRice16_9 is a macro calling a function to reduce code size.
#define TRice16_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) TRice16fn_10(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9))                                             //!< TRice16_10 is a macro calling a function to reduce code size.
#define TRice16_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) TRice16fn_11(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10))                       //!< TRice16_11 is a macro calling a function to reduce code size.
#define TRice16_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) TRice16fn_12(tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10), (uint16_t)(v11)) //!< TRice16_12 is a macro calling a function to reduce code size.

#define trice32_0(tid, fmt) trice32fn_0(tid)                                                                                                                                                                                                                                                       //!< trice32_0 is a macro calling a function to reduce code size.
#define trice32_1(tid, fmt, v0) trice32fn_1(tid, (uint32_t)(v0))                                                                                                                                                                                                                                   //!< trice32_1 is a macro calling a function to reduce code size.
#define trice32_2(tid, fmt, v0, v1) trice32fn_2(tid, (uint32_t)(v0), (uint32_t)(v1))                                                                                                                                                                                                               //!< trice32_2 is a macro calling a function to reduce code size.
#define trice32_3(tid, fmt, v0, v1, v2) trice32fn_3(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2))                                                                                                                                                                                           //!< trice32_3 is a macro calling a function to reduce code size.
#define trice32_4(tid, fmt, v0, v1, v2, v3) trice32fn_4(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3))                                                                                                                                                                       //!< trice32_4 is a macro calling a function to r  educe code size.
#define trice32_5(tid, fmt, v0, v1, v2, v3, v4) trice32fn_5(tid, (uint32_t)v0, (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4))                                                                                                                                                     //!< trice32_5 is a macro calling a function to reduce code size.
#define trice32_6(tid, fmt, v0, v1, v2, v3, v4, v5) trice32fn_6(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5))                                                                                                                               //!< trice32_6 is a macro calling a function to reduce code size.
#define trice32_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) trice32fn_7(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6))                                                                                                           //!< trice32_7 is a macro calling a function to reduce code size.
#define trice32_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) trice32fn_8(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7))                                                                                       //!< trice32_8 is a macro calling a function to reduce code size.
#define trice32_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) trice32fn_9(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8))                                                                   //!< trice32_9 is a macro calling a function to reduce code size.
#define trice32_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) trice32fn_10(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9))                                             //!< trice32_10 is a macro calling a function to reduce code size.
#define trice32_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) trice32fn_11(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10))                       //!< trice32_11 is a macro calling a function to reduce code size.
#define trice32_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) trice32fn_12(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10), (uint32_t)(v11)) //!< trice32_12 is a macro calling a function to reduce code size.

#define Trice32_0(tid, fmt) Trice32fn_0(tid)                                                                                                                                                                                                                                                       //!< Trice32_0 is a macro calling a function to reduce code size.
#define Trice32_1(tid, fmt, v0) Trice32fn_1(tid, (uint32_t)(v0))                                                                                                                                                                                                                                   //!< Trice32_1 is a macro calling a function to reduce code size.
#define Trice32_2(tid, fmt, v0, v1) Trice32fn_2(tid, (uint32_t)(v0), (uint32_t)(v1))                                                                                                                                                                                                               //!< Trice32_2 is a macro calling a function to reduce code size.
#define Trice32_3(tid, fmt, v0, v1, v2) Trice32fn_3(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2))                                                                                                                                                                                           //!< Trice32_3 is a macro calling a function to reduce code size.
#define Trice32_4(tid, fmt, v0, v1, v2, v3) Trice32fn_4(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3))                                                                                                                                                                       //!< Trice32_4 is a macro calling a function to reduce code size.
#define Trice32_5(tid, fmt, v0, v1, v2, v3, v4) Trice32fn_5(tid, (uint32_t)v0, (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4))                                                                                                                                                     //!< Trice32_5 is a macro calling a function to reduce code size.
#define Trice32_6(tid, fmt, v0, v1, v2, v3, v4, v5) Trice32fn_6(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5))                                                                                                                               //!< Trice32_6 is a macro calling a function to reduce code size.
#define Trice32_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) Trice32fn_7(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6))                                                                                                           //!< Trice32_7 is a macro calling a function to reduce code size.
#define Trice32_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) Trice32fn_8(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7))                                                                                       //!< Trice32_8 is a macro calling a function to reduce code size.
#define Trice32_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) Trice32fn_9(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8))                                                                   //!< Trice32_9 is a macro calling a function to reduce code size.
#define Trice32_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) Trice32fn_10(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9))                                             //!< Trice32_10 is a macro calling a function to reduce code size.
#define Trice32_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) Trice32fn_11(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10))                       //!< Trice32_11 is a macro calling a function to reduce code size.
#define Trice32_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) Trice32fn_12(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10), (uint32_t)(v11)) //!< Trice32_12 is a macro calling a function to reduce code size.

#define TRice32_0(tid, fmt) TRice32fn_0(tid)                                                                                                                                                                                                                                                       //!< TRice32_1 is a macro calling a function to reduce code size.
#define TRice32_1(tid, fmt, v0) TRice32fn_1(tid, (uint32_t)(v0))                                                                                                                                                                                                                                   //!< TRice32_1 is a macro calling a function to reduce code size.
#define TRice32_2(tid, fmt, v0, v1) TRice32fn_2(tid, (uint32_t)(v0), (uint32_t)(v1))                                                                                                                                                                                                               //!< TRice32_2 is a macro calling a function to reduce code size.
#define TRice32_3(tid, fmt, v0, v1, v2) TRice32fn_3(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2))                                                                                                                                                                                           //!< TRice32_3 is a macro calling a function to reduce code size.
#define TRice32_4(tid, fmt, v0, v1, v2, v3) TRice32fn_4(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3))                                                                                                                                                                       //!< TRice32_4 is a macro calling a function to reduce code size.
#define TRice32_5(tid, fmt, v0, v1, v2, v3, v4) TRice32fn_5(tid, (uint32_t)v0, (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4))                                                                                                                                                     //!< TRice32_5 is a macro calling a function to reduce code size.
#define TRice32_6(tid, fmt, v0, v1, v2, v3, v4, v5) TRice32fn_6(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5))                                                                                                                               //!< TRice32_6 is a macro calling a function to reduce code size.
#define TRice32_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) TRice32fn_7(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6))                                                                                                           //!< TRice32_7 is a macro calling a function to reduce code size.
#define TRice32_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) TRice32fn_8(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7))                                                                                       //!< TRice32_32_M is a macro calling a function to reduce code size.
#define TRice32_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) TRice32fn_9(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8))                                                                   //!< TRice32_9 is a macro calling a function to reduce code size.
#define TRice32_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) TRice32fn_10(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9))                                             //!< TRice32_10 is a macro calling a function to reduce code size.
#define TRice32_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) TRice32fn_11(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10))                       //!< TRice32_11 is a macro calling a function to reduce code size.
#define TRice32_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) TRice32fn_12(tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10), (uint32_t)(v11)) //!< TRice32_12 is a macro calling a function to reduce code size.

#define trice64_0(tid, fmt) trice64fn_0(tid)                                                                                                                                                                                                                                                       //!< trice64_1 is a macro calling a function to reduce code size.
#define trice64_1(tid, fmt, v0) trice64fn_1(tid, (uint64_t)(v0))                                                                                                                                                                                                                                   //!< trice64_1 is a macro calling a function to reduce code size.
#define trice64_2(tid, fmt, v0, v1) trice64fn_2(tid, (uint64_t)(v0), (uint64_t)(v1))                                                                                                                                                                                                               //!< trice64_2 is a macro calling a function to reduce code size.
#define trice64_3(tid, fmt, v0, v1, v2) trice64fn_3(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2))                                                                                                                                                                                           //!< trice64_3 is a macro calling a function to reduce code size.
#define trice64_4(tid, fmt, v0, v1, v2, v3) trice64fn_4(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3))                                                                                                                                                                       //!< trice64_4 is a macro calling a function to reduce code size.
#define trice64_5(tid, fmt, v0, v1, v2, v3, v4) trice64fn_5(tid, (uint64_t)v0, (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4))                                                                                                                                                     //!< trice64_5 is a macro calling a function to reduce code size.
#define trice64_6(tid, fmt, v0, v1, v2, v3, v4, v5) trice64fn_6(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5))                                                                                                                               //!< trice64_6 is a macro calling a function to reduce code size.
#define trice64_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) trice64fn_7(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6))                                                                                                           //!< trice64_7 is a macro calling a function to reduce code size.
#define trice64_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) trice64fn_8(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7))                                                                                       //!< trice64_8 is a macro calling a function to reduce code size.
#define trice64_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) trice64fn_9(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8))                                                                   //!< trice64_9 is a macro calling a function to reduce code size.
#define trice64_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) trice64fn_10(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9))                                             //!< trice64_10 is a macro calling a function to reduce code size.
#define trice64_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) trice64fn_11(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9), (uint64_t)(v10))                       //!< trice64_11 is a macro calling a function to reduce code size.
#define trice64_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) trice64fn_12(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9), (uint64_t)(v10), (uint64_t)(v11)) //!< trice64_12 is a macro calling a function to reduce code size.

#define Trice64_0(tid, fmt) Trice64fn_0(tid)                                                                                                                                                                                                                                                       //!< Trice64_0 is a macro calling a function to reduce code size.
#define Trice64_1(tid, fmt, v0) Trice64fn_1(tid, (uint64_t)(v0))                                                                                                                                                                                                                                   //!< Trice64_1 is a macro calling a function to reduce code size.
#define Trice64_2(tid, fmt, v0, v1) Trice64fn_2(tid, (uint64_t)(v0), (uint64_t)(v1))                                                                                                                                                                                                               //!< Trice64_2 is a macro calling a function to reduce code size.
#define Trice64_3(tid, fmt, v0, v1, v2) Trice64fn_3(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2))                                                                                                                                                                                           //!< Trice64_3 is a macro calling a function to reduce code size.
#define Trice64_4(tid, fmt, v0, v1, v2, v3) Trice64fn_4(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3))                                                                                                                                                                       //!< Trice64_4 is a macro calling a function to reduce code size.
#define Trice64_5(tid, fmt, v0, v1, v2, v3, v4) Trice64fn_5(tid, (uint64_t)v0, (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4))                                                                                                                                                     //!< Trice64_5 is a macro calling a function to reduce code size.
#define Trice64_6(tid, fmt, v0, v1, v2, v3, v4, v5) Trice64fn_6(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5))                                                                                                                               //!< Trice64_6 is a macro calling a function to reduce code size.
#define Trice64_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) Trice64fn_7(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6))                                                                                                           //!< Trice64_7 is a macro calling a function to reduce code size.
#define Trice64_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) Trice64fn_8(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7))                                                                                       //!< Trice64_8 is a macro calling a function to reduce code size.
#define Trice64_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) Trice64fn_9(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8))                                                                   //!< Trice64_9 is a macro calling a function to reduce code size.
#define Trice64_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) Trice64fn_10(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9))                                             //!< Trice64_10 is a macro calling a function to reduce code size.
#define Trice64_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) Trice64fn_11(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9), (uint64_t)(v10))                       //!< Trice64_11 is a macro calling a function to reduce code size.
#define Trice64_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) Trice64fn_12(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9), (uint64_t)(v10), (uint64_t)(v11)) //!< Trice64_12 is a macro calling a function to reduce code size.

#define TRice64_0(tid, fmt) TRice64fn_0(tid)                                                                                                                                                                                                                                                       //!< TRice64_0 is a macro calling a function to reduce code size.
#define TRice64_1(tid, fmt, v0) TRice64fn_1(tid, (uint64_t)(v0))                                                                                                                                                                                                                                   //!< TRice64_1 is a macro calling a function to reduce code size.
#define TRice64_2(tid, fmt, v0, v1) TRice64fn_2(tid, (uint64_t)(v0), (uint64_t)(v1))                                                                                                                                                                                                               //!< TRice64_2 is a macro calling a function to reduce code size.
#define TRice64_3(tid, fmt, v0, v1, v2) TRice64fn_3(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2))                                                                                                                                                                                           //!< TRice64_3 is a macro calling a function to reduce code size.
#define TRice64_4(tid, fmt, v0, v1, v2, v3) TRice64fn_4(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3))                                                                                                                                                                       //!< TRice64_4 is a macro calling a function to reduce code size.
#define TRice64_5(tid, fmt, v0, v1, v2, v3, v4) TRice64fn_5(tid, (uint64_t)v0, (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4))                                                                                                                                                     //!< TRice64_5 is a macro calling a function to reduce code size.
#define TRice64_6(tid, fmt, v0, v1, v2, v3, v4, v5) TRice64fn_6(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5))                                                                                                                               //!< TRice64_6 is a macro calling a function to reduce code size.
#define TRice64_7(tid, fmt, v0, v1, v2, v3, v4, v5, v6) TRice64fn_7(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6))                                                                                                           //!< TRice64_7 is a macro calling a function to reduce code size.
#define TRice64_8(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7) TRice64fn_8(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7))                                                                                       //!< TRice64_64_M is a macro calling a function to reduce code size.
#define TRice64_9(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8) TRice64fn_9(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8))                                                                   //!< TRice64_9 is a macro calling a function to reduce code size.
#define TRice64_10(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9) TRice64fn_10(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9))                                             //!< TRice64_10 is a macro calling a function to reduce code size.
#define TRice64_11(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10) TRice64fn_11(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9), (uint64_t)(v10))                       //!< TRice64_11 is a macro calling a function to reduce code size.
#define TRice64_12(tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11) TRice64fn_12(tid, (uint64_t)(v0), (uint64_t)(v1), (uint64_t)(v2), (uint64_t)(v3), (uint64_t)(v4), (uint64_t)(v5), (uint64_t)(v6), (uint64_t)(v7), (uint64_t)(v8), (uint64_t)(v9), (uint64_t)(v10), (uint64_t)(v11)) //!< TRice64_12 is a macro calling a function to reduce code size.
