// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint32_t id;
    unsigned line;
    int value;
} ExpectedWrapperSite;

static const ExpectedWrapperSite expectedWrapperSites[] = {
    {5101u, 300u, 7},
    {5101u, 301u, 8},
};

static size_t observedWrapperSites;
static int wrapperSiteFailures;

// pocWrapperEmit proves that nested macro expansion keeps the outer invocation line.
static void pocWrapperEmit(uint32_t id, unsigned line, const char *format, int value)
{
    const size_t count = sizeof expectedWrapperSites / sizeof expectedWrapperSites[0];

    if (observedWrapperSites >= count) {
        fprintf(stderr, "unexpected wrapper site: id=%u line=%u value=%d\n", id, line, value);
        wrapperSiteFailures++;
        return;
    }

    const ExpectedWrapperSite *expected = &expectedWrapperSites[observedWrapperSites];
    if (id != expected->id || line != expected->line || value != expected->value || strcmp(format, "error=%d") != 0) {
        fprintf(stderr,
                "wrapper site mismatch: got id=%u line=%u value=%d, expected id=%u line=%u value=%d\n",
                id,
                line,
                value,
                expected->id,
                expected->line,
                expected->value);
        wrapperSiteFailures++;
    }

    printf("wrapper-site id=%u line=%u value=%d\n", id, line, value);
    observedWrapperSites++;
}

// The unchanged production dispatch resolves __LINE__ when the public macro expands.
#define iD(n) ((uint32_t)(n))
#define TRICE_INSERT_trice(tid, format, value) \
    pocWrapperEmit((uint32_t)(tid), (unsigned)__LINE__, (format), (value))
#include "../../src/triceBind.h"

#define POC_WRAPPER_DESCRIPTORS
#include "poc_bind_descriptors.h"

#line 50 "wrapper_definition.h"
#define LOG_ERROR(value) trice("error=%d", value)

int main(void)
{
#line 300 "wrapper_owned.c"
    LOG_ERROR(7);
    LOG_ERROR(8);
#line 1000 "macro_definition.c"

    const size_t expectedCount = sizeof expectedWrapperSites / sizeof expectedWrapperSites[0];
    if (observedWrapperSites != expectedCount) {
        fprintf(stderr, "observed %zu wrapper sites, expected %zu\n", observedWrapperSites, expectedCount);
        wrapperSiteFailures++;
    }
    if (wrapperSiteFailures != 0) {
        return EXIT_FAILURE;
    }

    puts("macro-definition: passed");
    return EXIT_SUCCESS;
}
