// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ExpectedEmission describes one runtime-selected Trice definition.
typedef struct {
    const char *kind;
    uint32_t id;
    const char *format;
    int hasIntegerValue;
    int integerValue;
} ExpectedEmission;

// Every invocation expands both Trice definitions, but the switch executes one.
static const ExpectedEmission expectedEmissions[] = {
    {"trice", 6101u, "cannot open file\n", 0, 0},
    {"trice", 6102u, "error=%d", 1, 8},
    {"trice", 6101u, "cannot open file\n", 0, 0},
    {"trice", 6102u, "error=%d", 1, 8},
};

// These counters collect all mismatches so one run reports more than one error.
static size_t observedEmissions;
static int emissionFailures;

// pocEmit verifies that the generated local ordinal selected the stable ID of
// the definition that actually executes. It has no production-side behavior.
static void pocEmit(const char *kind,
                    uint32_t id,
                    const char *format,
                    int hasIntegerValue,
                    int integerValue)
{
    const size_t expectedCount = sizeof expectedEmissions / sizeof expectedEmissions[0];

    if (observedEmissions >= expectedCount) {
        fprintf(stderr, "unexpected emission: kind=%s id=%u\n", kind, (unsigned)id);
        emissionFailures++;
        return;
    }

    const ExpectedEmission *expected = &expectedEmissions[observedEmissions];
    if (strcmp(kind, expected->kind) != 0 || id != expected->id ||
        strcmp(format, expected->format) != 0 ||
        hasIntegerValue != expected->hasIntegerValue ||
        integerValue != expected->integerValue) {
        fprintf(stderr,
                "emission %zu mismatch: got kind=%s id=%u format=%s, "
                "expected kind=%s id=%u format=%s\n",
                observedEmissions,
                kind,
                (unsigned)id,
                format,
                expected->kind,
                (unsigned)expected->id,
                expected->format);
        emissionFailures++;
    }

    printf("emission[%zu]: kind=%s id=%u\n", observedEmissions, kind, (unsigned)id);
    observedEmissions++;
}

// These PoC emitters preserve the relevant zero- and one-value trice shapes.
#define iD(n) (n)
#define POC_TRICE_TEXT(tid, format) pocEmit("trice", (uint32_t)(tid), (format), 0, 0)
#define POC_TRICE_INTEGER(tid, format, integerValue) \
    pocEmit("trice", (uint32_t)(tid), (format), 1, (int)(integerValue))
#define POC_TRICE_SELECT(_format, _value, selected, ...) selected
#define TRICE_INSERT_trice(tid, ...) \
    POC_TRICE_SELECT(__VA_ARGS__, POC_TRICE_INTEGER, POC_TRICE_TEXT, unused)(tid, __VA_ARGS__)
#include "../../src/triceBind.h"

// This setup include models the file-level sidecar installed by `trice bind`.
#define TRICE_BIND_REBASE_SETUP
#include "trice_bind_rebase_generated.h"
#undef TRICE_BIND_REBASE_SETUP

// This is the requested user macro without IDs or bind-specific syntax.
#define LOG_ERROR(value)                                      \
    do {                                                      \
        switch (value) {                                      \
        case 0:                                               \
            break;                                            \
        case 7:                                               \
            trice("cannot open file\n");                     \
            break;                                            \
        default:                                              \
            trice("error=%d", 8);                             \
            break;                                            \
        }                                                     \
    } while (0)

#if defined(POC_FORCE_NO_COUNTER) || !defined(__COUNTER__)

// This isolated inclusion keeps the unsupported-compiler diagnostic concise.
#define TRICE_BIND_REBASE_SCOPE S_CASE0
#define TRICE_BIND_REBASE_BEGIN
#include "trice_bind_rebase_generated.h"
#undef TRICE_BIND_REBASE_BEGIN
#undef TRICE_BIND_REBASE_SCOPE

int main(void)
{
    return EXIT_FAILURE;
}

#else

int main(void)
{
    // These unrelated expansions prove that an absolute counter value is not
    // part of a generated descriptor and may vary before every local scope.
    enum { unrelatedBeforeCase0A = __COUNTER__, unrelatedBeforeCase0B = __COUNTER__ };
    (void)unrelatedBeforeCase0A;
    (void)unrelatedBeforeCase0B;

#define TRICE_BIND_REBASE_SCOPE S_CASE0
#define TRICE_BIND_REBASE_BEGIN
#include "trice_bind_rebase_generated.h"
#undef TRICE_BIND_REBASE_BEGIN
    LOG_ERROR(0);
#define TRICE_BIND_REBASE_END
#include "trice_bind_rebase_generated.h"
#undef TRICE_BIND_REBASE_END

    enum { unrelatedBeforeCase7 = __COUNTER__ };
    (void)unrelatedBeforeCase7;

#define TRICE_BIND_REBASE_SCOPE S_CASE7
#define TRICE_BIND_REBASE_BEGIN
#include "trice_bind_rebase_generated.h"
#undef TRICE_BIND_REBASE_BEGIN
#if defined(POC_INJECT_COUNTER_DRIFT)
    // The negative test must fail instead of silently selecting shifted IDs.
    enum { injectedCounterDrift = __COUNTER__ };
    (void)injectedCounterDrift;
#endif
    LOG_ERROR(7);
#define TRICE_BIND_REBASE_END
#include "trice_bind_rebase_generated.h"
#undef TRICE_BIND_REBASE_END

    enum { unrelatedBeforeDefaultA = __COUNTER__, unrelatedBeforeDefaultB = __COUNTER__ };
    (void)unrelatedBeforeDefaultA;
    (void)unrelatedBeforeDefaultB;

#define TRICE_BIND_REBASE_SCOPE S_DEFAULT
#define TRICE_BIND_REBASE_BEGIN
#include "trice_bind_rebase_generated.h"
#undef TRICE_BIND_REBASE_BEGIN
    LOG_ERROR(99);
#define TRICE_BIND_REBASE_END
#include "trice_bind_rebase_generated.h"
#undef TRICE_BIND_REBASE_END

    enum { unrelatedBeforeSameLine = __COUNTER__ };
    (void)unrelatedBeforeSameLine;

#define TRICE_BIND_REBASE_SCOPE S_SAME_LINE
#define TRICE_BIND_REBASE_BEGIN
#include "trice_bind_rebase_generated.h"
#undef TRICE_BIND_REBASE_BEGIN
    LOG_ERROR(7); LOG_ERROR(99);
#define TRICE_BIND_REBASE_END
#include "trice_bind_rebase_generated.h"
#undef TRICE_BIND_REBASE_END

    const size_t expectedCount = sizeof expectedEmissions / sizeof expectedEmissions[0];
    if (observedEmissions != expectedCount) {
        fprintf(stderr,
                "observed %zu emissions, expected %zu\n",
                observedEmissions,
                expectedCount);
        emissionFailures++;
    }
    if (emissionFailures != 0) {
        return EXIT_FAILURE;
    }

    printf("local bases may differ: %d, %d, %d, %d\n",
           triceBindCounterBase_S_CASE0,
           triceBindCounterBase_S_CASE7,
           triceBindCounterBase_S_DEFAULT,
           triceBindCounterBase_S_SAME_LINE);
    puts("variant-1 counter rebase: passed");
    return EXIT_SUCCESS;
}

#endif
