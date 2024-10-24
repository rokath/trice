/*! \file triceLogDiagnosticData.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#include "trice.h"

#ifdef __cplusplus
extern "C" {
#endif

#if TRICE_DIAGNOSTICS == 1

//! TriceLogDiagnosticData shows the max used buffer space.
void TriceLogDiagnosticData(void) {

#ifdef SEGGER_RTT
	int RTT0_wrSpaceMin = BUFFER_SIZE_UP - RTT0_writeDepthMax;
	if ((RTT0_wrSpaceMin < TRICE_BUFFER_SIZE)) {
		trice(iD(16300), "WARNING:RTT0_writeDepthMax=%u (BUFFER_SIZE_UP=%u)\n", RTT0_writeDepthMax, BUFFER_SIZE_UP);
	} else {
		trice(iD(16301), "diag:RTT0_writeDepthMax=%u (BUFFER_SIZE_UP=%u)\n", RTT0_writeDepthMax, BUFFER_SIZE_UP);
	}
#endif // #ifdef SEGGER_RTT

	if (TriceErrorCount > 0) {
		trice(iD(16302), "err: TriceErrorCount = %u\n", TriceErrorCount);
	}
#if TRICE_PROTECT == 1
	if (TriceDirectOverflowCount != 0) {
		trice(iD(16303), "err: TriceDirectOverflowCount = %u\n", TriceDirectOverflowCount);
	}
	if (TriceDeferredOverflowCount != 0) {
		trice(iD(16304), "err: TriceDeferredOverflowCount = %u\n", TriceDeferredOverflowCount);
	}
#endif

	unsigned triceSingleDepthMax = TRICE_DATA_OFFSET + (TriceSingleMaxWordCount << 2); // lint !e845 Info 845: The left argument to operator '<<' is certain to be 0
	if (triceSingleDepthMax <= TRICE_BUFFER_SIZE) {
		trice16(iD(16305), "diag:triceSingleDepthMax =%4u of %d (TRICE_BUFFER_SIZE)\n", triceSingleDepthMax, TRICE_BUFFER_SIZE);
	} else {
		trice16(iD(16306), "err:triceSingleDepthMax =%4u of %d (TRICE_BUFFER_SIZE overflow!)\n", triceSingleDepthMax, TRICE_BUFFER_SIZE);
	}

	if (TriceDataOffsetDepthMax < 0 || TriceDataOffsetDepthMax >= TRICE_DATA_OFFSET) {
		trice16(iD(16307), "err:TriceDataOffsetDepthMax = %d of %d (TRICE_DATA_OFFSET)\n", TriceDataOffsetDepthMax, TRICE_DATA_OFFSET);
	} else if (TriceDataOffsetDepthMax != 0) {
		trice16(iD(16308), "diag:TriceDataOffsetDepthMax = %d of %d (TRICE_DATA_OFFSET)\n", TriceDataOffsetDepthMax, TRICE_DATA_OFFSET);
	}

	if (TriceDynBufTruncateCount != 0) {
		trice8(iD(16309), "err: TriceDynBufTruncateCount = %u\n", TriceDynBufTruncateCount);
	}

#if TRICE_BUFFER == TRICE_STACK_BUFFER
#endif // #if TRICE_BUFFER == TRICE_STACK_BUFFER

#if TRICE_BUFFER == TRICE_STATIC_BUFFER
#endif // #if TRICE_BUFFER == TRICE_STATIC_BUFFER

#if TRICE_BUFFER == TRICE_DOUBLE_BUFFER
	if (TriceHalfBufferDepthMax <= TRICE_DEFERRED_BUFFER_SIZE / 2) {
		TRice16(iD(16310), "diag:TriceHalfBufferDepthMax =%4u of%5d\n", TriceHalfBufferDepthMax, TRICE_DEFERRED_BUFFER_SIZE / 2);
	} else {
		TRice16(iD(16311), "err:TriceHalfBufferDepthMax =%4u of%5d (overflow!)\n", TriceHalfBufferDepthMax, TRICE_DEFERRED_BUFFER_SIZE / 2);
	}
#endif // #if TRICE_BUFFER == TRICE_DOUBLE_BUFFER

#if TRICE_BUFFER == TRICE_RING_BUFFER
	if (TriceRingBufferDepthMax <= TRICE_DEFERRED_BUFFER_SIZE) {
		trice16(iD(16312), "diag:triceRingBufferDepthMax =%4u of%5d\n", TriceRingBufferDepthMax, TRICE_DEFERRED_BUFFER_SIZE);
	} else {
		trice16(iD(16313), "err:triceRingBufferDepthMax =%4u of%5d (overflow!)\n", TriceRingBufferDepthMax, TRICE_DEFERRED_BUFFER_SIZE);
	}
#endif // #if TRICE_BUFFER == TRICE_RING_BUFFER
}

#endif // #if TRICE_DIAGNOSTICS == 1
