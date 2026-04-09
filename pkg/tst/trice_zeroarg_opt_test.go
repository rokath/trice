// SPDX-License-Identifier: MIT

package tst

import (
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

const zeroArgTestConfig = `// SPDX-License-Identifier: MIT
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS
#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA
#define TRICE_CGO 1
#define TRICE_CYCLE_COUNTER 0
#endif
`

const zeroArgDispatcherSource = `// SPDX-License-Identifier: MIT
#include "trice.h"
void zero_arg_dispatcher(void) {
	trice(iD(0x1101), "ZEROARG_TRICE_DISPATCH");
	Trice(iD(0x1102), "ZEROARG_Trice_DISPATCH");
	TRice(iD(0x1103), "ZEROARG_TRice_DISPATCH");
}
`

const zeroArgExplicitSource = `// SPDX-License-Identifier: MIT
#include "trice.h"
void zero_arg_explicit(void) {
	trice0(iD(0x1201), "ZEROARG_trice0_EXPLICIT");
	Trice0(iD(0x1202), "ZEROARG_Trice0_EXPLICIT");
	TRice0(iD(0x1203), "ZEROARG_TRice0_EXPLICIT");
}
`

type zeroArgCompileCase struct {
	name           string
	std            string
	source         string
	mustNotContain []string
}

// availableCCompilers returns the locally usable C compilers for the artifact
// regression tests below.
//
// The tests are intentionally written to accept either host compilers or the
// ARM cross-compiler because the regression is about how the front-end emits
// object code for zero-value TRICE calls, not about target-specific execution.
func availableCCompilers() []string {
	candidates := []string{"arm-none-eabi-gcc", "gcc", "cc", "clang"}
	var found []string
	seen := map[string]bool{}
	for _, candidate := range candidates {
		path, err := exec.LookPath(candidate)
		if err != nil {
			continue
		}
		if seen[path] {
			continue
		}
		seen[path] = true
		found = append(found, candidate)
	}
	return found
}

// compileZeroArgSample compiles one small C translation unit and returns both
// the raw object bytes and the assembler text.
//
// We inspect both artifacts:
// - the object file proves whether the string literal survived in readonly data
// - the assembler proves whether the compiler still emits calls to the old
//   wrapper names such as trice0/Trice0/TRice0
//
// The temporary triceConfig.h keeps the sample self-contained and stable. It
// enables only the minimum TRICE configuration that is needed to compile the
// zero-value paths without depending on a full example project.
func compileZeroArgSample(t *testing.T, compiler string, tc zeroArgCompileCase) ([]byte, string) {
	t.Helper()

	tempDir := t.TempDir()
	srcDir, err := filepath.Abs(filepath.Join("..", "..", "src"))
	require.NoError(t, err)

	require.NoError(t, os.WriteFile(filepath.Join(tempDir, "triceConfig.h"), []byte(zeroArgTestConfig), 0o644))
	srcFile := filepath.Join(tempDir, "sample.c")
	require.NoError(t, os.WriteFile(srcFile, []byte(tc.source), 0o644))

	objFile := filepath.Join(tempDir, "sample.o")
	asmFile := filepath.Join(tempDir, "sample.s")

	compile := exec.Command(compiler,
		"-c", srcFile,
		"-I", tempDir,
		"-I", srcDir,
		"-O2",
		"-ffunction-sections",
		"-fdata-sections",
		"-std="+tc.std,
		"-o", objFile,
	)
	output, err := compile.CombinedOutput()
	require.NoErrorf(t, err, "%s", output)

	assemble := exec.Command(compiler,
		"-S", srcFile,
		"-I", tempDir,
		"-I", srcDir,
		"-O2",
		"-ffunction-sections",
		"-fdata-sections",
		"-std="+tc.std,
		"-o", asmFile,
	)
	output, err = assemble.CombinedOutput()
	require.NoErrorf(t, err, "%s", output)

	objBytes, err := os.ReadFile(objFile)
	require.NoError(t, err)
	asmBytes, err := os.ReadFile(asmFile)
	require.NoError(t, err)
	return objBytes, string(asmBytes)
}

// TestZeroArgDispatcherElidesFormatStrings guards the primary user-facing case:
// the inserted source form `trice(iD(...), "msg")`, `Trice(iD(...), "msg")`
// and `TRice(iD(...), "msg")`.
//
// Before the fix, these zero-value dispatcher calls ended up as calls to
// wrapper functions taking `const char* pFmt`. That forced compilers without
// LTO to keep the string literal in the object file. This test ensures that the
// dispatcher path now drops the literal before any function call is formed.
func TestZeroArgDispatcherElidesFormatStrings(t *testing.T) {
	compilers := availableCCompilers()
	if len(compilers) == 0 {
		t.Skip("no supported C compiler found")
	}

	tc := zeroArgCompileCase{
		name:   "dispatcher-gnu11",
		std:    "gnu11",
		source: zeroArgDispatcherSource,
		mustNotContain: []string{
			"ZEROARG_TRICE_DISPATCH",
			"ZEROARG_Trice_DISPATCH",
			"ZEROARG_TRice_DISPATCH",
		},
	}

	for _, compiler := range compilers {
		t.Run(compiler, func(t *testing.T) {
			objBytes, asmText := compileZeroArgSample(t, compiler, tc)
			for _, needle := range tc.mustNotContain {
				assert.NotContains(t, string(objBytes), needle)
				assert.NotContains(t, asmText, needle)
			}
			assert.NotContains(t, asmText, "bl\ttrice0")
			assert.NotContains(t, asmText, "bl\tTrice0")
			assert.NotContains(t, asmText, "bl\tTRice0")
		})
	}
}

// TestExplicitZeroArgMacrosElideFormatStrings guards the explicit compatibility
// spellings `trice0(...)`, `Trice0(...)` and `TRice0(...)`.
//
// These forms are still needed as fixed-arity fallbacks for strict non-GNU C
// modes where empty variadic argument handling is awkward. They must therefore
// stay source-compatible while still discarding the format string early enough
// to avoid leaving it in the object file.
func TestExplicitZeroArgMacrosElideFormatStrings(t *testing.T) {
	compilers := availableCCompilers()
	if len(compilers) == 0 {
		t.Skip("no supported C compiler found")
	}

	tc := zeroArgCompileCase{
		name:   "explicit-c11",
		std:    "c11",
		source: zeroArgExplicitSource,
		mustNotContain: []string{
			"ZEROARG_trice0_EXPLICIT",
			"ZEROARG_Trice0_EXPLICIT",
			"ZEROARG_TRice0_EXPLICIT",
		},
	}

	for _, compiler := range compilers {
		t.Run(compiler, func(t *testing.T) {
			objBytes, asmText := compileZeroArgSample(t, compiler, tc)
			for _, needle := range tc.mustNotContain {
				assert.NotContains(t, string(objBytes), needle)
				assert.NotContains(t, asmText, needle)
			}
			lowerAsm := strings.ToLower(asmText)
			assert.NotContains(t, lowerAsm, "trice0")
		})
	}
}
