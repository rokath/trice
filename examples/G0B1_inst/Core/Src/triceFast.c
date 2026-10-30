// SPDX-License-Identifier: MIT

// A bounded best-case logging benchmark; configuration 0 adds no executable code.
#include "trice.h"
#include "trice_sidecar_triceFast_c_0C5B5B5CE2EEBBF3.h" // trice-bind: keep as last include before this file's Trice calls

#if CONFIGURATION == 98 && !TRICE_OFF

// Sixteen straight-line calls expose burst throughput without loop overhead.
// Eight samples plus all reports need less than 1 KiB of the active buffer half.
enum { FAST_RECORDS = 16, FAST_SAMPLES = 8 };
_Static_assert(TRICE_DEFERRED_BUFFER_SIZE / 2 >= 1024 + TRICE_DATA_OFFSET + 4,
               "G0B1 fast records and reports must fit in one double-buffer half");

// Keep raw and empty-block timings so the reported correction remains visible.
typedef struct {
	uint32_t raw;
	uint32_t overhead;
} FastSample;

// Compiler-only barriers prevent record writes from moving across timer reads.
// The caller uses DSB as well to include completion of hardware memory stores.
static inline uint32_t fastClock(void) {
	__asm volatile("" ::: "memory");
	uint32_t clock = TIM2->CNT;
	__asm volatile("" ::: "memory");
	return clock;
}

// Reserve TIM2 for this profile, with no interrupt, external pin or prescaling.
// APB1 and the CPU both run at 64 MHz, so one TIM2 tick is one CPU clock.
void G0B1FastInit(void) {
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	TIM2->CR1 = 0;
	TIM2->DIER = 0;
	TIM2->SMCR = 0;
	TIM2->PSC = 0;
	TIM2->ARR = UINT32_MAX;
	TIM2->EGR = TIM_EGR_UG;
	TIM2->SR = 0;
	TIM2->CNT = 0;
	TIM2->CR1 = TIM_CR1_CEN;
}

// Run once from the only logging task, before any UART transfer is started.
// Interrupt writers are disabled in this profile; the other task stays quiet.
// All storage is bounded on the task stack, with no allocation or transport in
// a timed block. Unsigned subtraction handles one timer wrap for short samples.
void G0B1FastMeasure(void) {
	FastSample samples[FAST_SAMPLES];
	for (unsigned sample = 0; sample < FAST_SAMPLES; ++sample) {
		// Preserve the caller's mask rather than unconditionally enabling IRQs.
		uint32_t mask = __get_PRIMASK();
		__disable_irq();
		__DSB();
		uint32_t start = fastClock();
		__DSB();
		uint32_t overhead = fastClock() - start;
		__DSB();
		start = fastClock();
		trice("att:G0B1 fast record without timestamp\n");
		trice("att:G0B1 fast record without timestamp\n");
		trice("att:G0B1 fast record without timestamp\n");
		trice("att:G0B1 fast record without timestamp\n");
		trice("att:G0B1 fast record without timestamp\n");
		trice("att:G0B1 fast record without timestamp\n");
		trice("att:G0B1 fast record without timestamp\n");
		trice("att:G0B1 fast record without timestamp\n");
		trice("att:G0B1 fast record without timestamp\n");
		trice("att:G0B1 fast record without timestamp\n");
		trice("att:G0B1 fast record without timestamp\n");
		trice("att:G0B1 fast record without timestamp\n");
		trice("att:G0B1 fast record without timestamp\n");
		trice("att:G0B1 fast record without timestamp\n");
		trice("att:G0B1 fast record without timestamp\n");
		trice("att:G0B1 fast record without timestamp\n");
		__DSB();
		uint32_t elapsed = fastClock() - start;
		__set_PRIMASK(mask);
		samples[sample].raw = elapsed;
		samples[sample].overhead = overhead;
	}

	// Reports are appended only after every measurement. Decimal clock averages
	// use integer arithmetic; measurement noise cannot underflow the correction.
#if defined(__clang__)
	trice("msg:G0B1 fast compiler=Clang\n");
#else
	trice("msg:G0B1 fast compiler=GCC\n");
#endif
	trice("msg:G0B1 fast CPU/TIM2=%u Hz, samples=%u, records/sample=%u\n", SystemCoreClock, FAST_SAMPLES, FAST_RECORDS);
	for (unsigned sample = 0; sample < FAST_SAMPLES; ++sample) {
		uint32_t raw = samples[sample].raw;
		uint32_t overhead = samples[sample].overhead;
		uint32_t net = raw > overhead ? raw - overhead : 0;
		// Keep the Bind site on one physical line so GCC and Clang agree on __LINE__.
		trice("msg:G0B1 fast sample=%u raw=%u overhead=%u net=%u clocks/log=%u.%02u\n", sample, raw, overhead, net, net / FAST_RECORDS, (net % FAST_RECORDS) * 100u / FAST_RECORDS);
	}

	trice("deb:trice function call macros with 32-bit timestamp need ~54 clocks (reference)\n");
	TRice("msg:G0B1 TRice 0\n");
	TRice("msg:G0B1 TRice 1\n");
	TRice("msg:G0B1 TRice 2\n");
	TRice("msg:G0B1 TRice 3\n");
	TRice("msg:G0B1 TRice 4\n");
	TRice("msg:G0B1 TRice 5\n");
	TRice("msg:G0B1 TRice 6\n");

	trice("deb:trice function call macros with 16-bit timestamp need ~52 clocks (reference)\n");
	Trice( "msg:G0B1 Trice 0\n");
	Trice( "msg:G0B1 Trice 1\n");
	Trice( "msg:G0B1 Trice 2\n");
	Trice( "msg:G0B1 Trice 3\n");
	Trice( "msg:G0B1 Trice 4\n");
	Trice( "msg:G0B1 Trice 5\n");
	Trice( "msg:G0B1 Trice 6\n");
	
	trice("deb:trice insert macros with 32-bit timestamp need down to ~9 clocks (fastest)\n");
	TRICE(ID(0), "time:G0B1 TRICE(ID(0)... 0\n");
	TRICE(ID(0), "time:G0B1 TRICE(ID(0)... 1\n");
	TRICE(ID(0), "time:G0B1 TRICE(ID(0)... 2\n");
	TRICE(ID(0), "time:G0B1 TRICE(ID(0)... 3\n");
	TRICE(ID(0), "time:G0B1 TRICE(ID(0)... 4\n");
	TRICE(ID(0), "time:G0B1 TRICE(ID(0)... 5\n");
	TRICE(ID(0), "time:G0B1 TRICE(ID(0)... 6\n");

	trice("deb:trice insert macros with 16-bit timestamp need down to ~10 clocks (nearly fastest)\n");
	TRICE(Id(0), "time:G0B1 TRICE(Id(0)... 0\n");
	TRICE(Id(0), "time:G0B1 TRICE(Id(0)... 1\n");
	TRICE(Id(0), "time:G0B1 TRICE(Id(0)... 2\n");
	TRICE(Id(0), "time:G0B1 TRICE(Id(0)... 3\n");
	TRICE(Id(0), "time:G0B1 TRICE(Id(0)... 4\n");
	TRICE(Id(0), "time:G0B1 TRICE(Id(0)... 5\n");
	TRICE(Id(0), "time:G0B1 TRICE(Id(0)... 6\n");

}

#endif
