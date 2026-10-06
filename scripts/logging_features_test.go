// SPDX-License-Identifier: MIT

package scripts_test

import (
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
)

// loggingFeatureFixture runs the real selector and copy logic against tiny
// versioned projects. Tool doubles keep this orchestration suite independent
// of host Clang/ARM installations; the actual example checks run in step 515.
func loggingFeatureFixture(t *testing.T) (string, map[string]string) {
	t.Helper()
	root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_515_test_logging_features.sh")
	initGitFixture(t, root)
	for _, tool := range []string{"clang", "clang++", "clangd", "cc", "gcc", "trice", "make", "arm-none-eabi-gcc", "arm-none-eabi-objcopy", "arm-none-eabi-size"} {
		writeFixture(t, root, "bin/"+tool, "#!/bin/sh\nexit 0\n")
	}
	writeFixture(t, root, "bin/go", `#!/usr/bin/env bash
set -eu
printf '%s | integration=%s\n' "$*" "${TRICE_BIND_INTEGRATION:-unset}" >> "$LOG_DIR/go-calls"
[ "${TRICE_BIND_INTEGRATION:-}" = 1 ] || exit 9
case "${MOCK_GO_RESULT:-pass}" in
  fail) echo 'FAIL: fixture compiler error'; exit 7 ;;
  empty) echo 'testing: warning: no tests to run'; exit 0 ;;
  skipped_parent) echo '--- SKIP: TestContextEnrichmentTargetToDecoder (0.00s)'; exit 0 ;;
  skipped_child) echo '    --- SKIP: TestContextEnrichmentTargetToDecoder/c (0.00s)' ;;
esac
case "$2" in
  ./internal/args) tests='TestContextEnrichmentTargetToDecoder TestContextInsertCleanTargetToDecoder TestOrderedIDsTargetRecordsMatchSourceAndCatalog' ;;
  ./internal/id) tests='TestContextEnrichmentPoC TestContextEnrichmentPoCRebaseScopeBoundary' ;;
  *) echo 'FAIL: unexpected package selection'; exit 8 ;;
esac
for test in $tests; do
  if [ "${MOCK_GO_RESULT:-}" = missing_second ] && [ "$test" = TestContextInsertCleanTargetToDecoder ]; then continue; fi
  if [ "${MOCK_GO_RESULT:-}" = missing_ordered_ids ] && [ "$test" = TestOrderedIDsTargetRecordsMatchSourceAndCatalog ]; then continue; fi
  printf '%s\n' "--- PASS: $test (0.01s)"
done
`)
	// Stage known source bytes, then change them below. Copying HEAD or the
	// index instead of the current worktree would make these examples fail.
	writeFixture(t, root, "src/trice.h", "tracked library header\n")
	writeFixture(t, root, "examples/PC_features/main.c", "staged source\n")
	writeFixture(t, root, "examples/PC_features/til.json", "staged dictionary\n")
	writeFixture(t, root, "examples/G0B1_features/Core/Src/main.c", "staged source\n")
	writeFixture(t, root, "examples/exampleData/triceExamples.c", "shared source\n")
	writeFixture(t, root, "examples/PC_features/check_output.sh", `#!/usr/bin/env bash
set -eu
cd "$(dirname "$0")"
grep -q 'local source edit' main.c
grep -q 'local dictionary edit' til.json
[ ! -e generated/user.h ] && [ ! -e capture.bin ] && [ ! -e pc_features ]
echo 'PC example ran' >> "$LOG_DIR/example-calls"
echo 'instrumented private source' > main.c
echo 'regenerated private dictionary' > til.json
mkdir -p generated
echo 'private sidecar' > generated/adapter.h
echo 'private capture' > capture.bin
echo 'PASS: fixture PC output checks'
exit "${MOCK_PC_STATUS:-0}"
`)
	writeFixture(t, root, "examples/G0B1_features/check_build.sh", `#!/usr/bin/env bash
set -eu
cd "$(dirname "$0")"
[ ! -e out.gcc/cached.o ] && [ ! -e generated/user.h ]
echo 'G0B1 example ran' >> "$LOG_DIR/example-calls"
echo 'instrumented private source' > Core/Src/main.c
echo 'bound private shared source' > ../exampleData/triceExamples.c
mkdir -p out.gcc generated
echo 'private sidecar' > generated/adapter.h
for artifact in G0B1.elf G0B1.hex G0B1.bin; do
  if [ "${MOCK_ARTIFACT:-}" = "$artifact" ]; then continue; fi
  echo 'firmware' > "out.gcc/$artifact"
done
if [ "${MOCK_ARTIFACT:-}" = empty ]; then : > out.gcc/G0B1.elf; fi
echo 'PASS: fixture G0B1 build checks'
exit "${MOCK_G0B1_STATUS:-0}"
`)
	out, err := exec.Command("git", "-C", root, "add", "src", "examples").CombinedOutput()
	if !assert.NoError(t, err, "%s", out) {
		t.FailNow()
	}
	// These user-owned inputs and old outputs must survive every outcome.
	preserved := map[string]string{
		"src/trice.h":                             "tracked library header\n",
		"examples/PC_features/main.c":             "local source edit\n",
		"examples/PC_features/til.json":           "local dictionary edit\n",
		"examples/PC_features/generated/user.h":   "user selection\n",
		"examples/PC_features/capture.bin":        "user capture\n",
		"examples/PC_features/pc_features":        "old user executable\n",
		"examples/G0B1_features/Core/Src/main.c":  "staged source\n",
		"examples/G0B1_features/out.gcc/cached.o": "old user object\n",
		"examples/G0B1_features/generated/user.h": "user MCU selection\n",
		"examples/exampleData/triceExamples.c":    "shared source\n",
	}
	for path, content := range preserved {
		writeFixture(t, root, path, content)
	}
	return root, preserved
}

// TestLoggingFeaturesSelectionAndIsolation proves real archive isolation and
// fail-fast behavior, including cancellation-compatible exits and missing
// artifacts after an otherwise successful example script.
func TestLoggingFeaturesSelectionAndIsolation(t *testing.T) {
	// All files and child environments belong to this suite's private fixtures.
	t.Parallel()
	for _, tc := range []struct {
		name, variable, value string
		status                int
		mcuRan                bool
	}{
		{"all_checks_pass", "", "", 0, true},
		{"PC_failure_stops_before_MCU_build", "MOCK_PC_STATUS", "23", 23, false},
		{"PC_interrupt_keeps_signal_exit", "MOCK_PC_STATUS", "130", 130, false},
		{"MCU_failure_retains_private_outputs", "MOCK_G0B1_STATUS", "24", 24, true},
		{"MCU_termination_keeps_signal_exit", "MOCK_G0B1_STATUS", "143", 143, true},
		{"missing_ELF_is_not_a_pass", "MOCK_ARTIFACT", "G0B1.elf", 1, true},
		{"missing_HEX_is_not_a_pass", "MOCK_ARTIFACT", "G0B1.hex", 1, true},
		{"missing_BIN_is_not_a_pass", "MOCK_ARTIFACT", "G0B1.bin", 1, true},
		{"empty_firmware_is_not_a_pass", "MOCK_ARTIFACT", "empty", 1, true},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root, preserved := loggingFeatureFixture(t)
			env := map[string]string{}
			if tc.variable != "" {
				env[tc.variable] = tc.value
			}
			out, err := runFixture(t, root, "./scripts/_515_test_logging_features.sh full", env)
			if tc.status == 0 {
				assert.NoError(t, err, out)
				assert.Contains(t, out, "MCU execution requires a separate board test")
			} else if assert.Error(t, err, out) {
				assert.Equal(t, tc.status, err.(*exec.ExitError).ExitCode(), out)
			}
			calls, err := os.ReadFile(filepath.Join(root, "temp/log/go-calls"))
			assert.NoError(t, err)
			lines := strings.Split(strings.TrimSpace(string(calls)), "\n")
			assert.Len(t, lines, 2, "only the two focused Go selections run")
			assert.Contains(t, string(calls), "test ./internal/args -run ^(TestContextEnrichmentTargetToDecoder|TestContextInsertCleanTargetToDecoder|TestOrderedIDsTargetRecordsMatchSourceAndCatalog)$ -count=1 -v | integration=1")
			assert.Contains(t, string(calls), "test ./internal/id -run ^(TestContextEnrichmentPoC|TestContextEnrichmentPoCRebaseScopeBoundary)$ -count=1 -v | integration=1")
			assert.NotContains(t, string(calls), "TestContextEnrichmentRebasePoC")
			calls, err = os.ReadFile(filepath.Join(root, "temp/log/example-calls"))
			assert.NoError(t, err)
			assert.Contains(t, string(calls), "PC example ran")
			assert.Equal(t, tc.mcuRan, strings.Contains(string(calls), "G0B1 example ran"))
			for path, want := range preserved {
				got, err := os.ReadFile(filepath.Join(root, path))
				assert.NoError(t, err)
				assert.Equal(t, want, string(got), path)
			}
			copies, err := filepath.Glob(filepath.Join(root, "temp/log/logging-features.*"))
			assert.NoError(t, err)
			if tc.status == 0 {
				assert.Empty(t, copies, "successful copies must not accumulate")
			} else {
				assert.Len(t, copies, 1, "failed sources and outputs remain inspectable")
			}
		})
	}
}

// TestLoggingFeaturesRequireActualGoPasses rejects zero-exit runs with absent
// or skipped mandatory tests. Neither condition may launch the example builds.
func TestLoggingFeaturesRequireActualGoPasses(t *testing.T) {
	t.Parallel()
	for _, outcome := range []string{"empty", "skipped_parent", "skipped_child", "missing_second", "missing_ordered_ids", "fail"} {
		t.Run(outcome, func(t *testing.T) {
			root, _ := loggingFeatureFixture(t)
			out, err := runFixture(t, root, "./scripts/_515_test_logging_features.sh full", map[string]string{"MOCK_GO_RESULT": outcome})
			assert.Error(t, err, out)
			assert.Contains(t, out, "FAIL:")
			_, err = os.Stat(filepath.Join(root, "temp/log/example-calls"))
			assert.True(t, os.IsNotExist(err), "stop the failing step before downstream builds")
		})
	}
}

// TestLoggingFeaturesMissingTools distinguishes a useful partial developer run
// from full release evidence. Override only command discovery, not execution,
// so these cases are independent of which tools happen to be installed locally.
func TestLoggingFeaturesMissingTools(t *testing.T) {
	t.Parallel()
	for _, missing := range []string{"go", "clang", "clang++", "clangd", "cc gcc", "trice", "git", "tar", "make", "arm-none-eabi-gcc", "arm-none-eabi-objcopy", "arm-none-eabi-size"} {
		for _, mode := range []string{"quick", "full"} {
			t.Run(mode+"_missing_"+missing, func(t *testing.T) {
				root, _ := loggingFeatureFixture(t)
				out, err := runFixture(t, root, `command() {
  if [ "$1" = -v ]; then
    case " $MOCK_MISSING_TOOL " in *" $2 "*) return 1 ;; esac
  fi
  builtin command "$@"
}
export -f command
./scripts/_515_test_logging_features.sh `+mode, map[string]string{"MOCK_MISSING_TOOL": missing})
				// When both host alternatives are absent, gcc is the attempted
				// fallback and must be named in the missing-tool diagnostic.
				assert.Contains(t, out, "MISSING TOOL: "+strings.TrimPrefix(missing, "cc "))
				if mode == "quick" {
					assert.NoError(t, err, out)
					assert.Contains(t, out, "SKIP:")
					assert.NotContains(t, out, "FAIL:")
				} else {
					assert.Error(t, err, out)
					assert.Contains(t, out, "FAIL:")
					assert.NotContains(t, out, "SKIP:")
				}
			})
		}
	}
}

// TestLoggingFeaturesAreSelectedExactlyOnce checks the real quick/full plans,
// including the explicit mode argument that makes missing tools fatal in full.
func TestLoggingFeaturesAreSelectedExactlyOnce(t *testing.T) {
	t.Parallel()
	for _, mode := range []string{"quick", "full"} {
		t.Run(mode, func(t *testing.T) {
			root := scriptFixture(t, "scripts/_100_test_common.sh", "scripts/_110_test_runner.sh")
			out, err := runFixture(t, root, `source scripts/_110_test_runner.sh
TEST_ALL_SELECTED=`+mode+`
build_test_plan
for ((i=0; i<${#TEST_ALL_PLAN_SCRIPTS[@]}; i++)); do
  printf '%s %s\n' "${TEST_ALL_PLAN_SCRIPTS[i]}" "${TEST_ALL_PLAN_ARGUMENTS[i]}"
done`, nil)
			assert.NoError(t, err, out)
			assert.Equal(t, 1, strings.Count(out, "_515_test_logging_features.sh "+mode))
			assert.Less(t, strings.Index(out, "_480_test_build_trice_tool.sh"), strings.Index(out, "_515_test_logging_features.sh"))
		})
	}
}
