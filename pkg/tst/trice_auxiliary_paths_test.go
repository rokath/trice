// SPDX-License-Identifier: MIT

package tst

import (
	"os"
	"os/exec"
	"path/filepath"
	"testing"

	"github.com/stretchr/testify/require"
)

const auxiliaryPathHarnessSource = `// SPDX-License-Identifier: MIT
#include <stdint.h>
#include <stddef.h>
#include "trice.h"

static unsigned callbackHit;

static void write8Auxiliary(const uint8_t* enc, size_t encLen) {
	if (encLen == 3u && enc[0] == 1u && enc[1] == 2u && enc[2] == 3u) {
		callbackHit = 1u;
	}
}

static void write32Auxiliary(const uint32_t* enc, unsigned count) {
	if (count == 2u && enc[0] == 11u && enc[1] == 22u) {
		callbackHit = 1u;
	}
}

int main(void) {
#if TRICE_DIRECT_AUXILIARY8 == 1
	uint8_t data[] = {1u, 2u, 3u};
	UserNonBlockingDirectWrite8AuxiliaryFn = write8Auxiliary;
	TriceNonBlockingDirectWrite8Auxiliary(data, sizeof data);
#elif TRICE_DEFERRED_AUXILIARY8 == 1
	uint8_t data[] = {1u, 2u, 3u};
	UserNonBlockingDeferredWrite8AuxiliaryFn = write8Auxiliary;
	TriceNonBlockingDeferredWrite8Auxiliary(data, sizeof data);
#elif TRICE_DIRECT_AUXILIARY32 == 1
	uint32_t data[] = {11u, 22u};
	UserNonBlockingDirectWrite32AuxiliaryFn = write32Auxiliary;
	TriceNonBlockingDirectWrite32Auxiliary(data, 2u);
#elif TRICE_DEFERRED_AUXILIARY32 == 1
	uint32_t data[] = {11u, 22u};
	UserNonBlockingDeferredWrite32AuxiliaryFn = write32Auxiliary;
	TriceNonBlockingDeferredWrite32Auxiliary(data, 2u);
#else
	return 99;
#endif
	return callbackHit == 1u ? 0 : 1;
}
`

const directAuxiliaryTestConfig = `#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_CYCLE_COUNTER 0
`

const deferredAuxiliaryTestConfig = `#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_CYCLE_COUNTER 0
`

type auxiliaryPathCase struct {
	name   string
	define string
	config string
}

// TestTriceAuxiliaryCallbackPaths compiles and executes each auxiliary backend
// path with exactly one TRICE_*_AUXILIARY* switch enabled. The executable sets
// the public callback pointer and fails if the matching helper does not invoke
// that callback with the original payload.
func TestTriceAuxiliaryCallbackPaths(t *testing.T) {
	compiler := hostCCompiler(t)
	if compiler == "" {
		t.Skip("no host C compiler available")
	}

	for _, tc := range []auxiliaryPathCase{
		{name: "direct8", define: "TRICE_DIRECT_AUXILIARY8", config: directAuxiliaryTestConfig},
		{name: "deferred8", define: "TRICE_DEFERRED_AUXILIARY8", config: deferredAuxiliaryTestConfig},
		{name: "direct32", define: "TRICE_DIRECT_AUXILIARY32", config: directAuxiliaryTestConfig},
		{name: "deferred32", define: "TRICE_DEFERRED_AUXILIARY32", config: deferredAuxiliaryTestConfig},
	} {
		t.Run(tc.name, func(t *testing.T) {
			compileAndRunAuxiliaryPathHarness(t, compiler, tc)
		})
	}
}

// hostCCompiler returns a compiler that can build an executable for the current
// host. Cross-compilers are intentionally excluded because the test runs the
// generated executable to prove the callback path, not only compilation.
func hostCCompiler(t *testing.T) string {
	t.Helper()

	for _, candidate := range []string{"gcc", "cc", "clang"} {
		if _, err := exec.LookPath(candidate); err == nil {
			return candidate
		}
	}
	return ""
}

// compileAndRunAuxiliaryPathHarness creates a temporary triceConfig.h with one
// auxiliary define enabled, links the small harness with triceAuxiliary.c, and
// executes the result so callback invocation is checked at runtime.
func compileAndRunAuxiliaryPathHarness(t *testing.T, compiler string, tc auxiliaryPathCase) {
	t.Helper()

	tempDir := t.TempDir()
	srcDir, err := filepath.Abs(filepath.Join("..", "..", "src"))
	require.NoError(t, err)

	config := "#ifndef TRICE_CONFIG_H_\n#define TRICE_CONFIG_H_\n" + tc.config + "#define " + tc.define + " 1\n#define TRICE_CGO 0\n#endif\n"
	require.NoError(t, os.WriteFile(filepath.Join(tempDir, "triceConfig.h"), []byte(config), 0o644))

	harnessFile := filepath.Join(tempDir, "auxiliary_path_harness.c")
	require.NoError(t, os.WriteFile(harnessFile, []byte(auxiliaryPathHarnessSource), 0o644))

	exeFile := filepath.Join(tempDir, "auxiliary_path_harness")
	if filepath.Ext(os.Args[0]) == ".exe" {
		exeFile += ".exe"
	}

	compile := exec.Command(compiler,
		"-std=c99",
		"-I", tempDir,
		"-I", srcDir,
		harnessFile,
		filepath.Join(srcDir, "triceAuxiliary.c"),
		"-o", exeFile,
	)
	output, err := compile.CombinedOutput()
	require.NoErrorf(t, err, "%s", output)

	run := exec.Command(exeFile)
	output, err = run.CombinedOutput()
	require.NoErrorf(t, err, "%s", output)
}
