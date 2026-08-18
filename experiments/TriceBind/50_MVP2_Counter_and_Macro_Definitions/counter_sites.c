// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __COUNTER__
#error "counter_sites.c requires a preprocessor providing __COUNTER__."
#endif

typedef struct {
	uint32_t id;
	unsigned line;
	const char* text;
} ExpectedCounterSite;

static const ExpectedCounterSite expectedCounterSites[] = {
    {4101u, 100u, "first"},
    {4102u, 100u, "second"},
    {4103u, 100u, "third"},
    {4104u, 200u, "fourth"},
    {4105u, 200u, "fifth"},
};

static size_t observedCounterSites;
static int counterSiteFailures;
static uint32_t lineOnlyIds[2];
static size_t observedLineOnlySites;
static uint32_t explicitOrdinalIds[2];
static size_t observedExplicitOrdinalSites;

// pocCounterEmit verifies that every expansion selected its own generated descriptor.
static void pocCounterEmit(uint32_t id, unsigned line, const char* text) {
	const size_t count = sizeof expectedCounterSites / sizeof expectedCounterSites[0];

	if (observedCounterSites >= count) {
		fprintf(stderr, "unexpected counter site: id=%u line=%u text=%s\n", id, line, text);
		counterSiteFailures++;
		return;
	}

	const ExpectedCounterSite* expected = &expectedCounterSites[observedCounterSites];
	if (id != expected->id || line != expected->line || strcmp(text, expected->text) != 0) {
		fprintf(stderr,
		        "counter site mismatch: got id=%u line=%u text=%s, expected id=%u line=%u text=%s\n",
		        id,
		        line,
		        text,
		        expected->id,
		        expected->line,
		        expected->text);
		counterSiteFailures++;
	}

	printf("counter-site id=%u line=%u text=%s\n", id, line, text);
	observedCounterSites++;
}

// pocLineOnlyEmit records the unavoidable collision of an unchanged line-only lookup.
static void pocLineOnlyEmit(uint32_t id, unsigned line, const char* text) {
	if (observedLineOnlySites < sizeof lineOnlyIds / sizeof lineOnlyIds[0]) {
		lineOnlyIds[observedLineOnlySites] = id;
	}
	observedLineOnlySites++;
	printf("line-only-site id=%u line=%u text=%s\n", id, line, text);
}

// pocExplicitOrdinalEmit verifies the generated-shadow-source alternative.
static void pocExplicitOrdinalEmit(uint32_t id, unsigned line, const char* text) {
	if (observedExplicitOrdinalSites < sizeof explicitOrdinalIds / sizeof explicitOrdinalIds[0]) {
		explicitOrdinalIds[observedExplicitOrdinalSites] = id;
	}
	observedExplicitOrdinalSites++;
	printf("explicit-ordinal-site id=%u line=%u text=%s\n", id, line, text);
}

// The real target implementation remains unchanged; only the PoC emitter replaces it.
#define iD(n) ((uint32_t)(n))
#define TRICE_INSERT_trice(tid, text) pocCounterEmit((uint32_t)(tid), (unsigned)__LINE__, (text))
#include "../../../src/triceBind.h"

// This candidate extends the current (file key, line) lookup with one counter component.
#define POC_BIND_COUNTER_SITE_II(key, line, counter) TRICE_BIND_SITE_##key##_L##line##_C##counter
#define POC_BIND_COUNTER_SITE_I(key, line, counter) POC_BIND_COUNTER_SITE_II(key, line, counter)
#define POC_BIND_COUNTER_SITE(key, line, counter) POC_BIND_COUNTER_SITE_I(key, line, counter)
#undef TRICE_BIND_SITE_HERE
#define TRICE_BIND_SITE_HERE() POC_BIND_COUNTER_SITE(TRICE_BIND_FILE_KEY, __LINE__, __COUNTER__)

#define POC_COUNTER_DESCRIPTORS
#include "poc_bind_descriptors.h"

// This negative control is exactly the current helper-header-only line lookup.
#define POC_LINE_ONLY_IMPLEMENTATION(tid, text) \
	pocLineOnlyEmit((uint32_t)(tid), (unsigned)__LINE__, (text))
#define POC_LINE_ONLY_TRICE(text) \
	TRICE_BIND_APPLY(TRICE_BIND_SITE(TRICE_BIND_FILE_KEY, __LINE__), POC_LINE_ONLY_IMPLEMENTATION, text)

// This positive control needs no counter but requires generated compiler input with ordinals.
#define POC_BIND_ORDINAL_SITE_II(key, line, ordinal) TRICE_BIND_SITE_##key##_L##line##_O##ordinal
#define POC_BIND_ORDINAL_SITE_I(key, line, ordinal) POC_BIND_ORDINAL_SITE_II(key, line, ordinal)
#define POC_BIND_ORDINAL_SITE(key, line, ordinal) POC_BIND_ORDINAL_SITE_I(key, line, ordinal)
#define POC_EXPLICIT_ORDINAL_IMPLEMENTATION(tid, text) \
	pocExplicitOrdinalEmit((uint32_t)(tid), (unsigned)__LINE__, (text))
#define POC_EXPLICIT_ORDINAL_TRICE_I(line, ordinal, text)                       \
	TRICE_BIND_APPLY(POC_BIND_ORDINAL_SITE(TRICE_BIND_FILE_KEY, line, ordinal), \
	                 POC_EXPLICIT_ORDINAL_IMPLEMENTATION,                       \
	                 text)
#define POC_EXPLICIT_ORDINAL_TRICE(ordinal, text) \
	POC_EXPLICIT_ORDINAL_TRICE_I(__LINE__, ordinal, text)

int main(void) {
#line 100 "counter_owned.c"
	trice("first");
	trice("second");
	trice("third");
	enum { unrelatedCounterConsumer = __COUNTER__ };
	(void)unrelatedCounterConsumer;
#line 200 "counter_owned.c"
	trice("fourth");
	trice("fifth");

#line 250 "unchanged_line_only.c"
	POC_LINE_ONLY_TRICE("collision-a");
	POC_LINE_ONLY_TRICE("collision-b");

#line 260 "generated_shadow.c"
	POC_EXPLICIT_ORDINAL_TRICE(0, "shadow-a");
	POC_EXPLICIT_ORDINAL_TRICE(1, "shadow-b");
#line 1000 "counter_sites.c"

	const size_t expectedCount = sizeof expectedCounterSites / sizeof expectedCounterSites[0];
	if (observedCounterSites != expectedCount) {
		fprintf(stderr, "observed %zu counter sites, expected %zu\n", observedCounterSites, expectedCount);
		counterSiteFailures++;
	}
	if (observedLineOnlySites != 2u || lineOnlyIds[0] != 4199u || lineOnlyIds[1] != 4199u) {
		fputs("line-only helper did not exhibit the expected same-line collision\n", stderr);
		counterSiteFailures++;
	} else {
		puts("line-only helper collision: confirmed");
	}
	if (observedExplicitOrdinalSites != 2u || explicitOrdinalIds[0] != 4201u ||
	    explicitOrdinalIds[1] != 4202u) {
		fputs("generated explicit ordinals did not select distinct descriptors\n", stderr);
		counterSiteFailures++;
	} else {
		puts("explicit ordinals without __COUNTER__: passed");
	}
	if (counterSiteFailures != 0) {
		return EXIT_FAILURE;
	}

	puts("counter-sites: passed");
	return EXIT_SUCCESS;
}
