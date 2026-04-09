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

const stringArgExplicitSource = `// SPDX-License-Identifier: MIT
#include "trice.h"
static char runtimeGeneratedBuffer[] = "runtime-payload";
void string_arg_explicit(void) {
	triceS(iD(0x1301), "STRINGARG_triceS_EXPLICIT", runtimeGeneratedBuffer);
	TriceS(iD(0x1302), "STRINGARG_TriceS_EXPLICIT", runtimeGeneratedBuffer);
	TRiceS(iD(0x1303), "STRINGARG_TRiceS_EXPLICIT", runtimeGeneratedBuffer);
}
`

// stringArgLegacyWrapperSource intentionally reconstructs the old problematic
// design locally inside the test translation unit.
//
// Why this source exists although the production headers no longer expose this
// pattern:
// - We want one regression test that exercises the fixed production path.
// - We also want one contrast test that proves the former wrapper structure is
//   indeed the thing that made optimization harder.
// - Rebuilding the old wrapper form here keeps that historical behavior
//   testable without reintroducing it into the actual library headers.
//
// The `#undef` lines are required because the current header deliberately
// defines `triceS`, `TriceS` and `TRiceS` as macros. For the contrast case we
// need plain function symbols with the old `fmt` parameter again.
//
// In other words:
// - `stringArgExplicitSource` models the fixed production path
// - `stringArgLegacyWrapperSource` models the old wrapper-based path
// Comparing both in the same test file gives a stable proof that the source
// structure, not just incidental compiler behavior, is what changed.
const stringArgLegacyWrapperSource = `// SPDX-License-Identifier: MIT
#include "trice.h"
#undef triceS
#undef TriceS
#undef TRiceS
static char runtimeGeneratedBuffer[] = "runtime-payload";
void triceS(int tid, const char* fmt, const char* runtimeGeneratedString) {
	TRICE_S(id(tid), fmt, runtimeGeneratedString);
}
void TriceS(int tid, const char* fmt, const char* runtimeGeneratedString) {
	TRICE_S(Id(tid), fmt, runtimeGeneratedString);
}
void TRiceS(int tid, const char* fmt, const char* runtimeGeneratedString) {
	TRICE_S(ID(tid), fmt, runtimeGeneratedString);
}
void string_arg_legacy_wrapper(void) {
	triceS(iD(0x1301), "STRINGARG_triceS_LEGACY", runtimeGeneratedBuffer);
	TriceS(iD(0x1302), "STRINGARG_TriceS_LEGACY", runtimeGeneratedBuffer);
	TRiceS(iD(0x1303), "STRINGARG_TRiceS_LEGACY", runtimeGeneratedBuffer);
}
`

type triceCompileCase struct {
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

// compileTriceSample compiles one small C translation unit and returns both the
// raw object bytes and the assembler text.
//
// We inspect both artifacts:
//   - the object file proves whether the string literal survived in readonly data
//   - the assembler proves whether the compiler still emits calls to the old
//     wrapper names such as trice0/Trice0/TRice0 or triceS/TriceS/TRiceS
//
// The temporary triceConfig.h keeps the sample self-contained and stable. It
// enables only the minimum TRICE configuration that is needed to compile the
// zero-value paths without depending on a full example project.
func compileTriceSample(t *testing.T, compiler string, tc triceCompileCase) ([]byte, string) {
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

	tc := triceCompileCase{
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
			objBytes, asmText := compileTriceSample(t, compiler, tc)
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

	tc := triceCompileCase{
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
			objBytes, asmText := compileTriceSample(t, compiler, tc)
			for _, needle := range tc.mustNotContain {
				assert.NotContains(t, string(objBytes), needle)
				assert.NotContains(t, asmText, needle)
			}
			lowerAsm := strings.ToLower(asmText)
			assert.NotContains(t, lowerAsm, "trice0")
		})
	}
}

// TestExplicitStringArgMacrosElideFormatStrings guards the fixed-arity source
// forms `triceS(...)`, `TriceS(...)` and `TRiceS(...)`.
//
// These calls must continue to pass the runtime-generated payload string into
// the backend, but they must not preserve the format string merely because it
// appeared in a wrapper call. The regression originally showed up on toolchains
// without LTO, so this test inspects the emitted object and assembler directly.
//
// What this test proves:
// - the unique format literals do not survive in the object data
// - the generated assembler does not expose the old wrapper symbols
//   `triceS`, `TriceS` or `TRiceS`
// - helper functions such as `triceSfn` are acceptable and expected because
//   they centralize the runtime-string work without carrying a `fmt` argument
func TestExplicitStringArgMacrosElideFormatStrings(t *testing.T) {
	compilers := availableCCompilers()
	if len(compilers) == 0 {
		t.Skip("no supported C compiler found")
	}

	tc := triceCompileCase{
		name:   "string-arg-c11",
		std:    "c11",
		source: stringArgExplicitSource,
		mustNotContain: []string{
			"STRINGARG_triceS_EXPLICIT",
			"STRINGARG_TriceS_EXPLICIT",
			"STRINGARG_TRiceS_EXPLICIT",
		},
	}

	for _, compiler := range compilers {
		t.Run(compiler, func(t *testing.T) {
			objBytes, asmText := compileTriceSample(t, compiler, tc)
			for _, needle := range tc.mustNotContain {
				assert.NotContains(t, string(objBytes), needle)
				assert.NotContains(t, asmText, needle)
			}
			assert.NotContains(t, asmText, "\n.global\ttriceS\n")
			assert.NotContains(t, asmText, "\n.global\tTriceS\n")
			assert.NotContains(t, asmText, "\n.global\tTRiceS\n")
			assert.NotContains(t, asmText, "\t.globl\t_triceS ")
			assert.NotContains(t, asmText, "\t.globl\t_TriceS ")
			assert.NotContains(t, asmText, "\t.globl\t_TRiceS ")
		})
	}
}

// TestLegacyStringArgWrappersKeepFormatStrings documents the pre-fix behavior:
// wrapper functions that accept `const char* fmt` keep the less-optimizable
// call structure alive.
//
// This test is intentionally the mirror image of the passing macro regression
// test above. Keeping it green proves that the optimized path is better
// because the macro structure changed, not because the toolchain suddenly got
// smarter.
//
// The expectations are intentionally split:
// - on all supported compilers we require the legacy wrapper symbols to remain
//   visible in assembler, because that is the structural difference
// - on the ARM GCC toolchain that originally triggered the issue we additionally
//   require the unique format literals to survive, which reproduces the user-
//   visible symptom from the issue report
func TestLegacyStringArgWrappersKeepFormatStrings(t *testing.T) {
	compilers := availableCCompilers()
	if len(compilers) == 0 {
		t.Skip("no supported C compiler found")
	}

	tc := triceCompileCase{
		name:   "string-arg-legacy-c11",
		std:    "c11",
		source: stringArgLegacyWrapperSource,
		mustNotContain: []string{
			"STRINGARG_triceS_LEGACY",
			"STRINGARG_TriceS_LEGACY",
			"STRINGARG_TRiceS_LEGACY",
		},
	}

	for _, compiler := range compilers {
		t.Run(compiler, func(t *testing.T) {
			objBytes, asmText := compileTriceSample(t, compiler, tc)
			assert.Contains(t, asmText, "triceS")
			assert.Contains(t, asmText, "TriceS")
			assert.Contains(t, asmText, "TRiceS")
			if compiler == "arm-none-eabi-gcc" {
				for _, needle := range tc.mustNotContain {
					assert.Contains(t, string(objBytes), needle)
					assert.Contains(t, asmText, needle)
				}
			}
		})
	}
}
