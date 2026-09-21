// SPDX-License-Identifier: MIT

package tst

import (
	"os"
	"os/exec"
	"path/filepath"
	"strconv"
	"testing"

	"github.com/stretchr/testify/require"
)

// routingHarnessSource counts calls made by one selected deferred output.
const routingHarnessSource = `// SPDX-License-Identifier: MIT
#include <stddef.h>
#include <stdint.h>
#define SEGGER_RTT_H
#define SEGGER_RTT_MAX_NUM_UP_BUFFERS 1
#define SEGGER_RTT_MAX_NUM_DOWN_BUFFERS 1
#define BUFFER_SIZE_UP 1024
#define BUFFER_SIZE_DOWN 0
#define SEGGER_RTT_PRINTF_BUFFER_SIZE 0
#include "trice.c"
#include "trice8.c"
#include "trice16.c"
#include "trice32.c"
#include "trice64.c"
#include "triceDoubleBuffer.c"
#include "triceRingBuffer.c"
#include "triceStackBuffer.c"
#include "triceStaticBuffer.c"
#include "xtea.c"
#include "cobsDecode.c"
#include "cobsEncode.c"
#include "tcobsv1Decode.c"
#include "tcobsv1Encode.c"
#include "triceRx.c"
#include "triceX0.c"

static unsigned routeHits;

void TriceNonBlockingWriteUartA(const void* buf, size_t len) {
	(void)buf;
	(void)len;
	routeHits++;
}

void TriceNonBlockingWriteUartB(const void* buf, size_t len) {
	(void)buf;
	(void)len;
	routeHits++;
}

void TriceNonBlockingDeferredWrite8Auxiliary(const uint8_t* buf, size_t len) {
	(void)buf;
	(void)len;
	routeHits++;
}

void TriceNonBlockingDeferredWrite32Auxiliary(const uint32_t* buf, unsigned count) {
	(void)buf;
	(void)count;
	routeHits++;
}

void TriceWriteDeviceCgo(const void* buf, unsigned len) {
	(void)buf;
	(void)len;
	routeHits++;
}

unsigned TriceOutDepthUartA(void) { return 0u; }
unsigned TriceOutDepthUartB(void) { return 0u; }
unsigned TriceOutDepthCGO(void) { return 0u; }

static void route(int id) {
	uint8_t encoded[8] = {1u, 2u, 3u};
	TriceNonBlockingDeferredWrite8(id, encoded, 3u);
}

int main(void) {
	route(ROUTE_MIN - 1);
	route(ROUTE_MIN);
#if ROUTE_MIN != ROUTE_MAX
	route(ROUTE_MIN + ((ROUTE_MAX - ROUTE_MIN) / 2));
	route(ROUTE_MAX);
#endif
	route(ROUTE_MAX + 1);
	return routeHits == EXPECTED_HITS ? 0 : (int)(10u + routeHits);
}
`

// routingTestConfig enables the smallest host-runnable deferred setup.
const routingTestConfig = `#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_CYCLE_COUNTER 0
#define TRICE_CONFIG_WARNINGS 0
#define TRICE_CGO 1
#endif
`

// routingCompileSource includes trice.c without linking so configuration
// diagnostics can be checked independently from target hardware functions.
const routingCompileSource = `// SPDX-License-Identifier: MIT
#include <stddef.h>
#include <stdint.h>
#define SEGGER_RTT_H
#define SEGGER_RTT_MAX_NUM_UP_BUFFERS 1
#define SEGGER_RTT_MAX_NUM_DOWN_BUFFERS 1
#define BUFFER_SIZE_UP 1024
#define BUFFER_SIZE_DOWN 0
#define SEGGER_RTT_PRINTF_BUFFER_SIZE 0
#include "trice.c"
`

// routingOutput defines one deferred output and the names of its range bounds.
type routingOutput struct {
	name    string // name identifies the output in test names.
	enable  string // enable is the output's preprocessor switch.
	minimum string // minimum is the lower-bound preprocessor switch.
	maximum string // maximum is the upper-bound preprocessor switch.
}

// routingOutputs lists every deferred output that supports ID routing.
var routingOutputs = []routingOutput{
	{name: "uarta", enable: "TRICE_DEFERRED_UARTA", minimum: "TRICE_UARTA_MIN_ID", maximum: "TRICE_UARTA_MAX_ID"},
	{name: "uartb", enable: "TRICE_DEFERRED_UARTB", minimum: "TRICE_UARTB_MIN_ID", maximum: "TRICE_UARTB_MAX_ID"},
	{name: "auxiliary8", enable: "TRICE_DEFERRED_AUXILIARY8", minimum: "TRICE_DEFERRED_AUXILIARY8_MIN_ID", maximum: "TRICE_DEFERRED_AUXILIARY8_MAX_ID"},
	{name: "auxiliary32", enable: "TRICE_DEFERRED_AUXILIARY32", minimum: "TRICE_DEFERRED_AUXILIARY32_MIN_ID", maximum: "TRICE_DEFERRED_AUXILIARY32_MAX_ID"},
	{name: "rtt8", enable: "TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE", minimum: "TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE_MIN_ID", maximum: "TRICE_DEFERRED_SEGGER_RTT_8BIT_WRITE_MAX_ID"},
}

// TestDeferredRoutingIncludesBothBounds compiles and runs every deferred
// output with a normal and a single-ID range.
func TestDeferredRoutingIncludesBothBounds(t *testing.T) {
	compiler := hostCCompiler(t)
	if compiler == "" {
		t.Skip("no host C compiler available")
	}

	for _, output := range routingOutputs {
		output := output
		for _, bounds := range []struct {
			name         string
			minimum      int
			maximum      int
			expectedHits int
		}{
			{name: "normal", minimum: 10, maximum: 20, expectedHits: 3},
			{name: "single ID", minimum: 10, maximum: 10, expectedHits: 1},
		} {
			bounds := bounds
			t.Run(output.name+"/"+bounds.name, func(t *testing.T) {
				compileAndRunRoutingHarness(t, compiler, output, bounds.minimum, bounds.maximum, bounds.expectedHits)
			})
		}
	}
}

// TestDeferredRoutingConfiguration validates the same activation and range
// contract for every supported deferred output.
func TestDeferredRoutingConfiguration(t *testing.T) {
	compiler := hostCCompiler(t)
	if compiler == "" {
		t.Skip("no host C compiler available")
	}

	for _, output := range routingOutputs {
		output := output
		cases := []struct {
			name       string
			bounds     []string
			wantError  bool
			diagnostic string
		}{
			{name: "missing bounds"},
			{name: "zero bounds", bounds: []string{"-D" + output.minimum + "=0", "-D" + output.maximum + "=0"}},
			{name: "minimum only", bounds: []string{"-D" + output.minimum + "=10"}, wantError: true, diagnostic: "must both be 0 or both be set"},
			{name: "maximum only", bounds: []string{"-D" + output.maximum + "=20"}, wantError: true, diagnostic: "must both be 0 or both be set"},
			{name: "valid range", bounds: []string{"-D" + output.minimum + "=10", "-D" + output.maximum + "=20"}},
			{name: "reversed range", bounds: []string{"-D" + output.minimum + "=20", "-D" + output.maximum + "=10"}, wantError: true, diagnostic: "must satisfy 1 <= MIN_ID <= MAX_ID <= 16383"},
			{name: "negative minimum", bounds: []string{"-D" + output.minimum + "=-1", "-D" + output.maximum + "=10"}, wantError: true, diagnostic: "must satisfy 1 <= MIN_ID <= MAX_ID <= 16383"},
			{name: "maximum above ID space", bounds: []string{"-D" + output.minimum + "=10", "-D" + output.maximum + "=16384"}, wantError: true, diagnostic: "must satisfy 1 <= MIN_ID <= MAX_ID <= 16383"},
		}
		for _, tc := range cases {
			tc := tc
			t.Run(output.name+"/"+tc.name, func(t *testing.T) {
				compileOutput, err := compileRoutingConfiguration(t, compiler, output, tc.bounds)
				if !tc.wantError {
					require.NoErrorf(t, err, "%s", compileOutput)
					return
				}
				require.Error(t, err)
				diagnostics := string(compileOutput)
				require.Contains(t, diagnostics, output.minimum)
				require.Contains(t, diagnostics, output.maximum)
				require.Contains(t, diagnostics, tc.diagnostic)
			})
		}

		t.Run(output.name+"/zero bounds route all IDs", func(t *testing.T) {
			compileAndRunRoutingHarness(t, compiler, output, 0, 0, 3)
		})
	}
}

// routingEnableDefinitions returns the output switch and any required UART
// hardware selector for one compile invocation.
func routingEnableDefinitions(output routingOutput) []string {
	definitions := []string{"-D" + output.enable + "=1"}
	if output.name == "uarta" {
		definitions = append(definitions, "-DTRICE_UARTA=1")
	}
	if output.name == "uartb" {
		definitions = append(definitions, "-DTRICE_UARTB=1")
	}
	return definitions
}

// compileRoutingConfiguration compiles trice.c with one output configuration
// and returns the complete compiler diagnostics for positive and negative cases.
func compileRoutingConfiguration(t *testing.T, compiler string, output routingOutput, bounds []string) ([]byte, error) {
	t.Helper()

	tempDir := t.TempDir()
	srcDir, err := filepath.Abs(filepath.Join("..", "..", "src"))
	require.NoError(t, err)
	require.NoError(t, os.WriteFile(filepath.Join(tempDir, "triceConfig.h"), []byte(routingTestConfig), 0o644))
	sourceFile := filepath.Join(tempDir, "routing_configuration.c")
	require.NoError(t, os.WriteFile(sourceFile, []byte(routingCompileSource), 0o644))

	arguments := []string{"-std=c99", "-I", tempDir, "-I", srcDir}
	arguments = append(arguments, routingEnableDefinitions(output)...)
	arguments = append(arguments, bounds...)
	arguments = append(arguments, "-c", sourceFile, "-o", filepath.Join(tempDir, "routing_configuration.o"))
	return exec.Command(compiler, arguments...).CombinedOutput()
}

// compileAndRunRoutingHarness builds one route with explicit bounds and runs
// the executable so the output stub observes the selected IDs.
func compileAndRunRoutingHarness(t *testing.T, compiler string, output routingOutput, minimum, maximum, expectedHits int) {
	t.Helper()

	tempDir := t.TempDir()
	srcDir, err := filepath.Abs(filepath.Join("..", "..", "src"))
	require.NoError(t, err)
	require.NoError(t, os.WriteFile(filepath.Join(tempDir, "triceConfig.h"), []byte(routingTestConfig), 0o644))
	harnessFile := filepath.Join(tempDir, "routing_harness.c")
	require.NoError(t, os.WriteFile(harnessFile, []byte(routingHarnessSource), 0o644))

	executable := filepath.Join(tempDir, "routing_harness")
	if filepath.Ext(os.Args[0]) == ".exe" {
		executable += ".exe"
	}
	definitions := routingEnableDefinitions(output)
	definitions = append(definitions,
		"-D"+output.minimum+"="+strconv.Itoa(minimum),
		"-D"+output.maximum+"="+strconv.Itoa(maximum),
		"-DROUTE_MIN="+strconv.Itoa(minimum),
		"-DROUTE_MAX="+strconv.Itoa(maximum),
		"-DEXPECTED_HITS="+strconv.Itoa(expectedHits),
	)
	arguments := []string{
		"-std=c99",
		"-ffunction-sections",
		"-fdata-sections",
		"-I", tempDir,
		"-I", srcDir,
	}
	arguments = append(arguments, definitions...)
	arguments = append(arguments, harnessFile, "-Wl,--gc-sections", "-o", executable)
	compileOutput, err := exec.Command(compiler, arguments...).CombinedOutput()
	require.NoErrorf(t, err, "%s", compileOutput)

	runOutput, err := exec.Command(executable).CombinedOutput()
	require.NoErrorf(t, err, "%s", runOutput)
}
