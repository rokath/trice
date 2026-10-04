// SPDX-License-Identifier: MIT

package scripts_test

import (
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
)

// l432Fixture runs the real scheduler in a disposable repo. The make double
// records complete arguments, observes overlapping jobs and simulates artifacts
// and compiler failures. The host ARM toolchain is never required by this test.
func l432Fixture(t *testing.T) string {
	t.Helper()
	root := scriptFixture(t, "examples/L432_inst/all_configs_build.sh")
	writeFixture(t, root, "examples/prepareTriceBind.sh", `prepare_trice_bind_build() {
  mkdir -p ../../events ../../active
  echo prepared >> ../../events/preparation
  [ "${PREPARATION_FAIL:-0}" = 0 ] || return 9
  export TRICE_ID_WORKFLOW_OWNER=1 TRICE_ID_WORKFLOW=bind
}
`)
	writeFixture(t, root, "scripts/_150_setup_build_environment.sh", "echo setup >> ../../events/setup\nexport MAKE_JOBS=\"${FIXTURE_MAKE_JOBS:--j}\"\n")
	writeFixture(t, root, "bin/getconf", "#!/bin/sh\nprintf '%s\\n' \"${FIXTURE_CPUS:-6}\"\n")
	writeFixture(t, root, "examples/L432_inst/out.gcc/user-object.o", "user build must survive\n")
	writeFixture(t, root, "shared-header.h", "first header version\n")
	writeFixture(t, root, "bin/make", `#!/usr/bin/env bash
set -eu
configuration= directory=
for arg; do
  case "$arg" in
    TRICE_FLAGS=*) configuration=${arg#TRICE_FLAGS=-DCONFIGURATION=}; configuration=${configuration% } ;;
    GCC_BUILD=*) directory=${arg#GCC_BUILD=} ;;
  esac
done
[ -n "$configuration" ] && [ -n "$directory" ] || exit 8
[ "$TRICE_ID_WORKFLOW_OWNER" = 1 ] && [ "$TRICE_ID_WORKFLOW" = bind ] || exit 8
printf '%s\n' "$@" > "../../events/$configuration.args"
cat ../../shared-header.h > "../../events/$configuration.header"
touch "../../active/$configuration"
trap 'rm -f "../../active/$configuration"' EXIT

# The first batch must overlap; later batches may not start before it drains.
if [ "${EXPECT_JOBS:-1}" -gt 1 ]; then
  if [ "$configuration" -eq 0 ]; then
    attempts=0
    until [ -f ../../events/overlap ]; do
      sleep 0.01
      attempts=$((attempts+1))
      if [ "$attempts" -gt 500 ]; then echo 'error: no overlapping configuration'; exit 1; fi
    done
  elif [ "$configuration" -eq "$((EXPECT_JOBS-1))" ]; then
    attempts=0
    until [ -f ../../active/0 ]; do
      sleep 0.01
      attempts=$((attempts+1))
      if [ "$attempts" -gt 500 ]; then echo 'error: first configuration not running'; exit 1; fi
    done
    touch ../../events/overlap
  elif [ "$configuration" -ge "$EXPECT_JOBS" ] && [ -f ../../active/0 ]; then
    echo 'error: concurrency limit exceeded'; exit 1
  fi
fi

if [ "${CANCEL_PROBE:-0}" = 1 ]; then
  bash -c 'trap "sleep 0.2; touch ../../events/child-stopped; exit 0" TERM; touch ../../events/child-ready; while :; do sleep 0.05; done' &
  wait "$!"
fi
if [ "$configuration" = "${FAIL_CONFIGURATION:-none}" ]; then
  echo 'Core/Src/main.c:42: error: deliberate compiler failure'
  echo 'a useful explanation of the compiler error'
  exit 7
fi
if [ "$configuration" = 0 ]; then echo 'Core/Src/main.c:10: warning: fixture warning'; fi
printf 'ELF\n' > "$directory/L432KC.elf"
printf 'HEX\n' > "$directory/L432KC.hex"
if [ "$configuration" != "${MISSING_ARTIFACT:-none}" ]; then printf 'BIN\n' > "$directory/L432KC.bin"; fi
`)
	return root
}

// TestL432MatrixPreservesEveryConfigurationAndBoundsConcurrency proves that
// scheduling alone changes: 0..100, the same define, gcc target, one make job,
// unique outputs and a single preparation regardless of the outer job count.
// Only the additional assembler text listings are disabled in matrix builds.
func TestL432MatrixPreservesEveryConfigurationAndBoundsConcurrency(t *testing.T) {
	for _, jobs := range []string{"1", "2", "default"} {
		t.Run(jobs, func(t *testing.T) {
			root := l432Fixture(t)
			expectedJobs := jobs
			if jobs == "default" {
				expectedJobs = "6"
			}
			out, err := runFixture(t, root, "bash examples/L432_inst/all_configs_build.sh", map[string]string{
				"TRICE_L432_TEST_JOBS": strings.Replace(jobs, "default", "", 1),
				"TRICE_TEST_NO_STOP":   "0", "EXPECT_JOBS": expectedJobs, "MAKEFLAGS": "-j99",
			})
			assert.NoError(t, err, out)
			assert.Contains(t, out, "101/101 configurations; failed=0")
			assert.Contains(t, out, "jobs="+expectedJobs+" make-jobs=1")
			assert.Contains(t, out, "fixture warning", "warnings remain visible without failing the matrix")
			assert.Contains(t, out, "completed with compiler warnings")
			for _, name := range []string{"preparation", "setup"} {
				data, err := os.ReadFile(filepath.Join(root, "events", name))
				assert.NoError(t, err)
				assert.Equal(t, 1, strings.Count(string(data), "\n"), name+" runs once")
			}
			for configuration := 0; configuration <= 100; configuration++ {
				data, err := os.ReadFile(filepath.Join(root, "events", fmt.Sprintf("%d.args", configuration)))
				if !assert.NoError(t, err) {
					continue
				}
				args := strings.Split(strings.TrimSuffix(string(data), "\n"), "\n")
				if !assert.Len(t, args, 5) {
					continue
				}
				assert.Equal(t, "-j1", args[0])
				assert.Equal(t, "GCC_LISTINGS=0", args[1])
				assert.True(t, strings.HasPrefix(args[2], "GCC_BUILD=../../temp/log/l432."), args[2])
				assert.True(t, strings.HasSuffix(args[2], fmt.Sprintf("/config-%d", configuration)), args[2])
				assert.Equal(t, fmt.Sprintf("TRICE_FLAGS=-DCONFIGURATION=%d ", configuration), args[3])
				assert.Equal(t, "gcc", args[4])
				_, err = os.Stat(filepath.Join(root, "examples/L432_inst", strings.TrimPrefix(args[2], "GCC_BUILD=")))
				assert.True(t, os.IsNotExist(err), "successful private build outputs are removed")
			}
			logs, err := filepath.Glob(filepath.Join(root, "temp/log/l432.*/*.log"))
			assert.NoError(t, err)
			assert.Len(t, logs, 101, "complete compiler logs survive successful cleanup")
			data, err := os.ReadFile(filepath.Join(root, "examples/L432_inst/out.gcc/user-object.o"))
			assert.NoError(t, err)
			assert.Equal(t, "user build must survive\n", string(data))
		})
	}
}

// TestL432ListingSwitchPreservesOtherCompilerOptions evaluates the actual Make
// fragment. Standalone builds keep their listings; matrix builds remove only
// that output option, including when other GCC-only options are added later.
func TestL432ListingSwitchPreservesOtherCompilerOptions(t *testing.T) {
	if _, err := exec.LookPath("make"); err != nil {
		t.Skip("make is required to evaluate the real L432 compiler options")
	}
	for _, tc := range []struct {
		name, arguments string
		listing         bool
	}{
		{"standalone_default_keeps_listings", "", true},
		{"explicit_listing_remains_available", "GCC_LISTINGS=1", true},
		{"matrix_omits_only_listing_option", "GCC_LISTINGS=0", false},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := scriptFixture(t, "examples/L432_inst/gcc0.mak")
			writeFixture(t, root, "fixture.c", "/* No compilation needed to inspect Make options. */\n")
			writeFixture(t, root, "Makefile", "include examples/L432_inst/gcc0.mak\nGCC_ONLY_FLAGS += -fother-option\nall: fixture.c\n\t@printf '%s\\n' '$(GCC_ONLY_FLAGS)'\n")
			out, err := runFixture(t, root, "make -s "+tc.arguments+" all", nil)
			assert.NoError(t, err, out)
			assert.Contains(t, out, "-fother-option")
			assert.Equal(t, tc.listing, strings.Contains(out, "-Wa,-a,-ad,-alms=out.gcc/fixture.lst"), out)
		})
	}
}

// TestL432MatrixFailureKeepsDiagnosticsAndHonorsStopPolicy verifies that a later
// success never erases a failure and fail-fast drains the current batch only.
func TestL432MatrixFailureKeepsDiagnosticsAndHonorsStopPolicy(t *testing.T) {
	for _, tc := range []struct {
		name, noStop, failure, missing, expected string
		count                                    int
	}{
		{"compiler_failure_stops_after_started_batch", "0", "1", "none", "main.c:42: error: deliberate compiler failure", 2},
		{"continue_reports_failure_even_when_last_configuration_passes", "1", "1", "none", "a useful explanation of the compiler error", 101},
		{"missing_binary_is_not_success", "0", "none", "1", "error: missing or empty L432 artifact:", 2},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := l432Fixture(t)
			out, err := runFixture(t, root, "bash examples/L432_inst/all_configs_build.sh", map[string]string{
				"TRICE_L432_TEST_JOBS": "2", "EXPECT_JOBS": "2", "TRICE_TEST_NO_STOP": tc.noStop,
				"FAIL_CONFIGURATION": tc.failure, "MISSING_ARTIFACT": tc.missing,
			})
			assert.Error(t, err, out)
			assert.Contains(t, out, "L432 FAIL CONFIGURATION=1")
			assert.Contains(t, out, tc.expected)
			assert.Contains(t, out, "./build.sh CONFIGURATION=1")
			assert.Contains(t, out, fmt.Sprintf("%d/101 configurations; failed=1", tc.count))
			logs, err := filepath.Glob(filepath.Join(root, "temp/log/l432.*/*.log"))
			assert.NoError(t, err)
			assert.Len(t, logs, tc.count)
			retained, err := filepath.Glob(filepath.Join(root, "temp/log/l432.*/config-1"))
			assert.NoError(t, err)
			assert.Len(t, retained, 1, "failed build remains available for inspection")
			active, err := os.ReadDir(filepath.Join(root, "active"))
			assert.NoError(t, err)
			assert.Empty(t, active, "all started make processes finish before restoration")
		})
	}
}

// TestL432MatrixRejectsBadControlsBeforePreparation ensures that invalid input
// cannot launch a partial matrix or modify shared Bind state.
func TestL432MatrixRejectsBadControlsBeforePreparation(t *testing.T) {
	for _, jobs := range []string{"0", "-1", "many", "01"} {
		t.Run(jobs, func(t *testing.T) {
			root := l432Fixture(t)
			out, err := runFixture(t, root, "bash examples/L432_inst/all_configs_build.sh", map[string]string{"TRICE_L432_TEST_JOBS": jobs})
			assert.Error(t, err)
			assert.Contains(t, out, "positive integer required")
			_, err = os.Stat(filepath.Join(root, "events/preparation"))
			assert.True(t, os.IsNotExist(err))
		})
	}
	root := l432Fixture(t)
	out, err := runFixture(t, root, "bash examples/L432_inst/all_configs_build.sh", map[string]string{"TRICE_TEST_NO_STOP": "invalid"})
	assert.Error(t, err)
	assert.Contains(t, out, "0 or 1 required")
	_, err = os.Stat(filepath.Join(root, "events/preparation"))
	assert.True(t, os.IsNotExist(err))
}

// TestL432AutomaticBudgetUsesPlatformLimits covers the shared Windows limit,
// online-CPU detection, a broken probe, and an explicit user override. Failure
// in configuration zero bounds these fixtures to one batch, not another matrix.
func TestL432AutomaticBudgetUsesPlatformLimits(t *testing.T) {
	for _, tc := range []struct {
		name, makeJobs, cpus, override, expected string
	}{
		{"bounded_platform_budget_precedes_logical_cpu_count", "-j2", "8", "", "2"},
		{"unlimited_make_becomes_online_cpu_budget", "-j", "6", "", "6"},
		{"invalid_cpu_probe_uses_safe_fallback", "-j", "unavailable", "", "4"},
		{"zero_cpu_count_uses_safe_fallback", "-j", "0", "", "4"},
		{"explicit_user_budget_precedes_detection", "-j2", "8", "1", "1"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := l432Fixture(t)
			out, err := runFixture(t, root, "bash examples/L432_inst/all_configs_build.sh", map[string]string{
				"TRICE_L432_TEST_JOBS": tc.override, "TRICE_TEST_NO_STOP": "0", "FAIL_CONFIGURATION": "0",
				"FIXTURE_MAKE_JOBS": tc.makeJobs, "FIXTURE_CPUS": tc.cpus,
			})
			assert.Error(t, err, out)
			assert.Contains(t, out, "jobs="+tc.expected+" make-jobs=1")
			assert.Contains(t, out, tc.expected+"/101 configurations; failed=1")
		})
	}
}

// TestL432MatrixPreparationFailureCannotLaunchCompilers protects the boundary
// between mutating source preparation and parallel read-only compilation.
func TestL432MatrixPreparationFailureCannotLaunchCompilers(t *testing.T) {
	root := l432Fixture(t)
	out, err := runFixture(t, root, "bash examples/L432_inst/all_configs_build.sh", map[string]string{"PREPARATION_FAIL": "1"})
	assert.Error(t, err, out)
	args, err := filepath.Glob(filepath.Join(root, "events/*.args"))
	assert.NoError(t, err)
	assert.Empty(t, args)
}

// TestL432MatrixNeverReusesOldBuildOutputs covers a warm second invocation after
// a shared header edit. Each configuration is executed again in a fresh directory
// instead of relying on incomplete external-header or toolchain cache signatures.
func TestL432MatrixNeverReusesOldBuildOutputs(t *testing.T) {
	root := l432Fixture(t)
	options := map[string]string{"TRICE_L432_TEST_JOBS": "2", "TRICE_TEST_NO_STOP": "0", "FAIL_CONFIGURATION": "1"}
	out, err := runFixture(t, root, "bash examples/L432_inst/all_configs_build.sh", options)
	assert.Error(t, err, out)
	first, err := os.ReadFile(filepath.Join(root, "events/0.args"))
	assert.NoError(t, err)
	writeFixture(t, root, "shared-header.h", "changed header version\n")
	out, err = runFixture(t, root, "bash examples/L432_inst/all_configs_build.sh", options)
	assert.Error(t, err, out)
	second, err := os.ReadFile(filepath.Join(root, "events/0.args"))
	assert.NoError(t, err)
	assert.NotEqual(t, string(first), string(second), "even successful configurations receive new build paths")
	for _, configuration := range []string{"0", "1"} {
		data, err := os.ReadFile(filepath.Join(root, "events", configuration+".header"))
		assert.NoError(t, err)
		assert.Equal(t, "changed header version\n", string(data))
	}
}

// TestL432CancellationStopsCompilerDescendants waits for a child handshake, then
// terminates the scheduler. It may return only after the compiler child stopped.
func TestL432CancellationStopsCompilerDescendants(t *testing.T) {
	if runtime.GOOS == "windows" {
		t.Skip("POSIX process-group signal assertion; ordinary scheduling is covered on Windows")
	}
	root := l432Fixture(t)
	out, err := runFixture(t, root, `bash examples/L432_inst/all_configs_build.sh > matrix.out 2>&1 &
worker=$!
attempts=0
until [ -f events/child-ready ]; do
  sleep 0.01
  attempts=$((attempts+1))
  if [ "$attempts" -gt 500 ]; then kill -TERM "$worker"; cat matrix.out; exit 1; fi
done
kill -TERM "$worker"
status=0
wait "$worker" || status=$?
cat matrix.out
printf 'MatrixExit:%s\n' "$status"
[ "$status" -eq 143 ] && [ -f events/child-stopped ]`, map[string]string{
		"TRICE_L432_TEST_JOBS": "1", "TRICE_TEST_NO_STOP": "1", "CANCEL_PROBE": "1",
	})
	assert.NoError(t, err, out)
	assert.Contains(t, out, "MatrixExit:143")
	_, err = os.Stat(filepath.Join(root, "events/1.args"))
	assert.True(t, os.IsNotExist(err), "cancellation must not schedule the next configuration")
}
