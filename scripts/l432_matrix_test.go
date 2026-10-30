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
			assert.NotContains(t, out, "Interrupted system call")
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

// l432ClangFixture supplies complete tool doubles, including a toolchain path
// containing spaces. These tests never invoke a host compiler or modify sources.
func l432ClangFixture(t *testing.T) string {
	t.Helper()
	root := scriptFixture(t, "examples/L432_inst/build_with_clang.sh", "examples/prepareTriceBind.sh")
	writeFixture(t, root, "trice_bindIDs_in_examples_and_test_folder.sh", `#!/bin/sh
echo prepared >> preparation.calls
[ "${PREPARATION_FAIL:-0}" = 0 ] || exit 9
mkdir -p generated
`)
	writeFixture(t, root, "tool chain/gcc/include/stdint.h", "/* fixture GCC headers */\n")
	writeFixture(t, root, "tool chain/arm-none-eabi/include/stdlib.h", "/* fixture Newlib headers */\n")
	for _, name := range []string{"libc.a", "libc_nano.a", "libm.a", "libnosys.a", "libgcc.a", "nano.specs"} {
		writeFixture(t, root, "tool chain/arm-none-eabi/lib/"+name, "fixture runtime file\n")
	}
	writeFixture(t, root, "examples/L432_inst/out.gcc/user-object.o", "keep GCC output\n")
	writeFixture(t, root, "bin/arm-none-eabi-gcc", `#!/bin/sh
root=$(cd "$(dirname "$0")/.." && pwd)
# Queries must use the firmware ABI and ignore unrelated host include variables.
case " $* " in *' -mfloat-abi=hard '*) ;; *) exit 98 ;; esac
[ -z "${CPATH:-}${C_INCLUDE_PATH:-}" ] || exit 97
query=
for argument; do
  case "$argument" in -dumpmachine | -print-file-name=* | -E) query=$argument ;; esac
done
runtime_dir="$root/tool chain/arm-none-eabi/lib"
header_dir="$root/tool chain/arm-none-eabi/include"
case "${FIXTURE_LAYOUT:-default}" in
  bundled) runtime_dir="$root/tool chain/gcc" ;;
  multilib) runtime_dir="$root/tool chain/arm-none-eabi/lib/thumb/v7e-m+fp/hard" ;;
  distribution) header_dir="$root/distribution/usr/include/newlib" ;;
  relocated) header_dir="$root/relocated sdk/sysroot/usr/include" ;;
esac
case "$query" in
  -dumpmachine) printf '%s\r\n' "${GCC_TARGET:-arm-none-eabi}" ;;
  -print-file-name=include) printf '%s\r\n' "$root/tool chain/gcc/include" ;;
  -print-file-name=*)
    name=${query#-print-file-name=}
    if [ "$name" = "${RUNTIME_MISSING:-none}" ]; then printf '%s\n' "$name"
    else printf '%s\r\n' "$runtime_dir/$name"; fi
    ;;
  -E)
    [ "${HEADER_SEARCH_STATUS:-0}" = 0 ] || { echo 'fixture preprocessor failure' >&2; exit 23; }
    printf '#include <...> search starts here:\r\n %s\r\n %s\r\nEnd of search list.\r\n' "$root/tool chain/gcc/include" "$header_dir" >&2
    ;;
  *) echo unexpected-GCC-invocation >&2; exit 99 ;;
esac
`)
	for _, tool := range []string{"trice", "arm-none-eabi-objcopy", "arm-none-eabi-size"} {
		writeFixture(t, root, "bin/"+tool, "#!/bin/sh\necho unexpected-tool-execution >&2\nexit 99\n")
	}
	writeFixture(t, root, "bin/clang", `#!/bin/sh
[ "$1" = --print-targets ] || exit 99
printf 'Registered Targets:\n    aarch64 - AArch64\n'
[ "${CLANG_NO_ARM:-0}" = 1 ] || printf '    arm - ARM\n    thumb - Thumb\n'
`)
	writeFixture(t, root, "bin/uname", "#!/bin/sh\nprintf '%s\\n' \"${FIXTURE_PLATFORM:-Linux}\"\n")
	// Conversion is simulated so every native host can exercise the Windows
	// selection path; the real Make tests below exercise native argument quoting.
	writeFixture(t, root, "bin/cygpath", "#!/bin/sh\necho converted >> ../../conversion.calls\nprintf '%s\\n' \"$2\"\n")
	writeFixture(t, root, "bin/make", `#!/bin/sh
if [ "${1:-}" = --version ]; then printf '%s\n' "${MAKE_VERSION:-GNU Make 4.4.1}"; exit 0; fi
printf '%s\n' "$@" > ../../make.calls
[ "${BUILD_STATUS:-0}" = 0 ] || { echo 'fixture compiler failure' >&2; exit "$BUILD_STATUS"; }
mkdir -p out.clang
for artifact in elf hex bin; do
  [ "$artifact" != "${MISSING_ARTIFACT:-none}" ] || continue
  printf '%s\n' "$@" > "out.clang/L432KC.$artifact"
done
`)
	return root
}

// TestL432ClangCLIHasNoPreparationSideEffects checks help and rejected arguments
// before header generation, including boundaries and accidental extra arguments.
func TestL432ClangCLIHasNoPreparationSideEffects(t *testing.T) {
	for _, tc := range []struct {
		arguments string
		help      bool
	}{
		{"--help", true}, {"-h", true}, {"", false}, {"-1", false},
		{"101", false}, {"098", false}, {"abc", false}, {"1 2", false},
	} {
		t.Run(tc.arguments, func(t *testing.T) {
			root := l432ClangFixture(t)
			out, err := runFixture(t, root, "bash examples/L432_inst/build_with_clang.sh "+tc.arguments, nil)
			assert.Equal(t, !tc.help, err != nil, out)
			if tc.help || tc.arguments == "" || tc.arguments == "1 2" {
				assert.Contains(t, out, "Usage:")
			} else {
				assert.Contains(t, out, "from 0 through 100")
			}
			_, err = os.Stat(filepath.Join(root, "preparation.calls"))
			assert.True(t, os.IsNotExist(err), "help and invalid arguments must not prepare sources")
			_, err = os.Stat(filepath.Join(root, "make.calls"))
			assert.True(t, os.IsNotExist(err), "help and invalid arguments must not build")
		})
	}
}

// TestL432ClangBuildsSelectedConfiguration covers all accepted number boundaries,
// cold/warm runs and simulated OS branches. Forced compilation must survive a
// configuration change while unrelated GCC artifacts remain byte-for-byte intact.
func TestL432ClangBuildsSelectedConfiguration(t *testing.T) {
	for _, platform := range []string{"Linux", "Darwin", "MINGW64_NT-10.0"} {
		t.Run(platform, func(t *testing.T) {
			root := l432ClangFixture(t)
			for _, configuration := range []string{"98", "1", "0", "100", "CONFIGURATION=98"} {
				out, err := runFixture(t, root, "bash examples/L432_inst/build_with_clang.sh "+configuration, map[string]string{
					"FIXTURE_PLATFORM": platform, "TRICE_ID_WORKFLOW_OWNER": "0", "MAKE_JOBS": "-j3", "CLANG_LIBC_INCLUDE": "",
				})
				assert.NoError(t, err, out)
				assert.Contains(t, out, "Build complete:")
				args, err := os.ReadFile(filepath.Join(root, "make.calls"))
				assert.NoError(t, err)
				assert.Contains(t, string(args), "-B\n-j3\nTRICE_FLAGS=-DCONFIGURATION="+strings.TrimPrefix(configuration, "CONFIGURATION=")+"\n")
				assert.Contains(t, string(args), "tool chain/gcc/include\n")
				assert.Contains(t, string(args), "tool chain/arm-none-eabi/include\n")
				assert.Contains(t, string(args), "TRICE_BIND_INCLUDE_DIR=")
				assert.True(t, strings.HasSuffix(string(args), "\nclang\n"), string(args))
				for _, extension := range []string{"elf", "hex", "bin"} {
					image, err := os.ReadFile(filepath.Join(root, "examples/L432_inst/out.clang/L432KC."+extension))
					assert.NoError(t, err)
					assert.Equal(t, string(args), string(image), "image must belong to this configuration")
				}
			}
			gcc, err := os.ReadFile(filepath.Join(root, "examples/L432_inst/out.gcc/user-object.o"))
			assert.NoError(t, err)
			assert.Equal(t, "keep GCC output\n", string(gcc))
			_, err = os.Stat(filepath.Join(root, "conversion.calls"))
			assert.Equal(t, strings.HasPrefix(platform, "MINGW"), err == nil, "only Windows needs native-path conversion")
		})
	}
}

// TestL432ClangFailuresRemainActionable ensures missing prerequisites fail before
// preparation, and preparation/compiler/image failures never report success.
func TestL432ClangFailuresRemainActionable(t *testing.T) {
	for _, tc := range []struct {
		name, remove, message string
		env                   map[string]string
	}{
		{"missing_library", "tool chain/arm-none-eabi/lib/libc.a", "ARM runtime file is unavailable", nil},
		{"missing_specs", "tool chain/arm-none-eabi/lib/nano.specs", "ARM runtime file is unavailable", nil},
		{"missing_headers", "tool chain/arm-none-eabi/include/stdlib.h", "stdlib.h was not found", nil},
		{"preparation_failure", "", "", map[string]string{"PREPARATION_FAIL": "1"}},
		{"compiler_failure", "", "fixture compiler failure", map[string]string{"BUILD_STATUS": "17"}},
		{"missing_image", "", "expected nonempty firmware image: out.clang/L432KC.bin", map[string]string{"MISSING_ARTIFACT": "bin"}},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := l432ClangFixture(t)
			if tc.remove != "" {
				assert.NoError(t, os.Remove(filepath.Join(root, tc.remove)))
			}
			// Explicit workflow ownership keeps the test independent of the caller.
			env := map[string]string{"TRICE_ID_WORKFLOW_OWNER": "0", "CLANG_LIBC_INCLUDE": ""}
			for key, value := range tc.env {
				env[key] = value
			}
			out, err := runFixture(t, root, "bash examples/L432_inst/build_with_clang.sh 98", env)
			assert.Error(t, err, out)
			assert.Contains(t, out, tc.message)
			assert.NotContains(t, out, "Build complete:")
			assert.Contains(t, out, "docs/rm/examples.md#l432-clang-prerequisites")
			if tc.remove != "" {
				_, err = os.Stat(filepath.Join(root, "preparation.calls"))
				assert.True(t, os.IsNotExist(err), "missing dependencies must not change source state")
			}
		})
	}
}

// TestL432ClangManagedWorkflowAndMissingTools verifies that an outer Insert
// workflow remains the owner and missing executables produce a useful error.
func TestL432ClangManagedWorkflowAndMissingTools(t *testing.T) {
	root := l432ClangFixture(t)
	out, err := runFixture(t, root, "bash examples/L432_inst/build_with_clang.sh 98", map[string]string{
		"TRICE_ID_WORKFLOW_OWNER": "1", "TRICE_ID_WORKFLOW": "insert", "TRICE_BIND_INCLUDE_DIR": "", "CLANG_LIBC_INCLUDE": "",
	})
	assert.NoError(t, err, out)
	_, err = os.Stat(filepath.Join(root, "preparation.calls"))
	assert.True(t, os.IsNotExist(err), "managed Insert sources must not be rebound")
	for _, tool := range []string{"make", "clang", "trice", "cygpath", "arm-none-eabi-gcc", "arm-none-eabi-objcopy", "arm-none-eabi-size"} {
		t.Run(tool, func(t *testing.T) {
			root := l432ClangFixture(t)
			// Hide one tool without relying on what is installed on the native host.
			out, err := runFixture(t, root, `command() {
  if [ "$1" = -v ] && [ "$2" = "$MISSING_TOOL" ]; then return 1; fi
  builtin command "$@"
}
export -f command
bash examples/L432_inst/build_with_clang.sh 98`, map[string]string{
				"MISSING_TOOL": tool, "TRICE_ID_WORKFLOW_OWNER": "0", "FIXTURE_PLATFORM": "MINGW64_NT-10.0",
			})
			assert.Error(t, err, out)
			assert.Contains(t, out, "not found on PATH: "+tool)
			assert.Contains(t, out, "PATH")
			assert.Contains(t, out, "docs/rm/examples.md#l432-clang-prerequisites")
		})
	}
}

// TestL432ClangDiscoversHeadersIndependentlyOfLibraryLayout reproduces the real
// Arm GNU failure: libc.a can be in GCC's own directory or a multilib subtree,
// while Newlib headers are elsewhere. Distribution/relocated layouts also use
// the reported search list rather than a hard-coded installation prefix.
func TestL432ClangDiscoversHeadersIndependentlyOfLibraryLayout(t *testing.T) {
	for _, layout := range []string{"default", "bundled", "multilib", "distribution", "relocated"} {
		t.Run(layout, func(t *testing.T) {
			root := l432ClangFixture(t)
			headerDir := "tool chain/arm-none-eabi/include"
			switch layout {
			case "bundled", "multilib":
				runtimeDir := "tool chain/gcc"
				if layout == "multilib" {
					runtimeDir = "tool chain/arm-none-eabi/lib/thumb/v7e-m+fp/hard"
				}
				for _, name := range []string{"libc.a", "libc_nano.a", "libm.a", "libnosys.a", "libgcc.a", "nano.specs"} {
					writeFixture(t, root, runtimeDir+"/"+name, "fixture runtime file\n")
				}
			case "distribution":
				headerDir = "distribution/usr/include/newlib"
			case "relocated":
				headerDir = "relocated sdk/sysroot/usr/include"
			}
			writeFixture(t, root, headerDir+"/stdlib.h", "/* fixture Newlib header */\n")
			out, err := runFixture(t, root, "bash examples/L432_inst/build_with_clang.sh --check", map[string]string{
				"FIXTURE_LAYOUT": layout, "CLANG_LIBC_INCLUDE": "", "TRICE_ID_WORKFLOW_OWNER": "0",
				"CPATH": "unrelated host headers", "C_INCLUDE_PATH": "unrelated generated headers",
			})
			assert.NoError(t, err, out)
			assert.Contains(t, out, "Prerequisites OK.")
			assert.Contains(t, out, "/"+headerDir+"\n")
			for _, marker := range []string{"preparation.calls", "make.calls"} {
				_, err := os.Stat(filepath.Join(root, marker))
				assert.True(t, os.IsNotExist(err), "--check must not prepare sources or build")
			}
		})
	}
}

// TestL432ClangPrerequisiteFailuresPointToManual covers capabilities as well as
// missing files. Every failure names the cause, links the central instructions,
// and stops before source preparation or a firmware build.
func TestL432ClangPrerequisiteFailuresPointToManual(t *testing.T) {
	for _, tc := range []struct {
		name, variable, value, message string
	}{
		{"wrong_make", "MAKE_VERSION", "BSD make", "not GNU make"},
		{"host_only_clang", "CLANG_NO_ARM", "1", "Thumb backend"},
		{"wrong_gcc_target", "GCC_TARGET", "x86_64-linux-gnu", "incompatible target"},
		{"missing_nano_library", "RUNTIME_MISSING", "libc_nano.a", "libc_nano.a"},
		{"missing_math_library", "RUNTIME_MISSING", "libm.a", "libm.a"},
		{"missing_syscall_stubs", "RUNTIME_MISSING", "libnosys.a", "libnosys.a"},
		{"missing_compiler_runtime", "RUNTIME_MISSING", "libgcc.a", "libgcc.a"},
		{"failed_header_query", "HEADER_SEARCH_STATUS", "23", "fixture preprocessor failure"},
		{"invalid_header_override", "CLANG_LIBC_INCLUDE", "missing headers", "CLANG_LIBC_INCLUDE does not contain stdlib.h"},
		{"unsupported_platform", "FIXTURE_PLATFORM", "FreeBSD", "unsupported build host"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := l432ClangFixture(t)
			env := map[string]string{"CLANG_LIBC_INCLUDE": "", "TRICE_ID_WORKFLOW_OWNER": "0"}
			env[tc.variable] = tc.value
			out, err := runFixture(t, root, "bash examples/L432_inst/build_with_clang.sh --check", env)
			assert.Error(t, err, out)
			assert.Contains(t, out, tc.message)
			assert.Contains(t, out, "docs/rm/examples.md#l432-clang-prerequisites")
			assert.NotContains(t, out, "Prerequisites OK.")
			for _, marker := range []string{"preparation.calls", "make.calls"} {
				_, err := os.Stat(filepath.Join(root, marker))
				assert.True(t, os.IsNotExist(err), "prerequisite failures must not prepare sources or build")
			}
		})
	}
}

// TestL432ClangExplicitHeadersBypassDiscovery retains an escape hatch for a
// custom SDK whose ARM headers are deliberately outside GCC's search list.
func TestL432ClangExplicitHeadersBypassDiscovery(t *testing.T) {
	root := l432ClangFixture(t)
	writeFixture(t, root, "custom headers/stdlib.h", "/* custom ARM Newlib headers */\n")
	out, err := runFixture(t, root, "bash examples/L432_inst/build_with_clang.sh --check", map[string]string{
		"CLANG_LIBC_INCLUDE":   filepath.ToSlash(filepath.Join(root, "custom headers")),
		"HEADER_SEARCH_STATUS": "23", "TRICE_ID_WORKFLOW_OWNER": "0",
	})
	assert.NoError(t, err, out)
	assert.Contains(t, out, "Prerequisites OK.")
	assert.Contains(t, out, "/custom headers\n")
	assert.NotContains(t, out, "fixture preprocessor failure")
}

// TestL432ClangMakeRules also exercises the GCC caller of the changed include
// list. Real Make expands the actual rules; tool doubles record argument boundaries
// and simulate images, so quoted paths and linker diagnostics are observable.
func TestL432ClangMakeRules(t *testing.T) {
	if _, err := exec.LookPath("make"); err != nil {
		t.Skip("GNU make is needed to evaluate the actual build rules")
	}
	for _, target := range []string{"clang", "gcc"} {
		for _, fail := range []string{"0", "1"} {
			t.Run(target+"/link_failure="+fail, func(t *testing.T) {
				root := scriptFixture(t, "examples/L432_inst/Makefile", "examples/L432_inst/flags.mak", "examples/L432_inst/mcu.mak",
					"examples/L432_inst/gcc0.mak", "examples/L432_inst/gcc1.mak", "examples/L432_inst/clang0.mak", "examples/L432_inst/clang1.mak")
				writeFixture(t, root, "examples/L432_inst/build.sh", "# unused by Make\n")
				writeFixture(t, root, "examples/L432_inst/probe.c", "/* fixture C input */\n")
				writeFixture(t, root, "examples/L432_inst/startup.s", "/* fixture assembly input */\n")
				// One tool double handles compilation, linking and binary conversion
				// without ever running a native ARM compiler or an ARM executable.
				toolDouble := `#!/bin/sh
case "$0" in
  *objcopy) for destination; do :; done; printf 'image\n' > "$destination"; exit 0 ;;
  *size) echo 'fixture size'; exit 0 ;;
esac
output= source= next=0
for argument; do
  if [ "$next" = 1 ]; then output=$argument; next=0; fi
  [ "$argument" != -o ] || next=1
  case "$argument" in *.c | *.s) source=$argument ;; esac
done
if [ -n "$source" ]; then
  printf '%s\n' "$@" > "$output.args"
  printf '%s\n' "$source" >> "$output.calls"
  printf 'object\n' > "$output"
else
  printf '%s\n' "$@" > "$output.args"
  echo 'fixture linker diagnostic' >&2
  [ "$LINK_FAIL" = 0 ] || exit 17
  printf 'ELF\n' > "$output"
fi
`
				for _, tool := range []string{"clang", "arm-none-eabi-gcc", "arm-none-eabi-objcopy", "arm-none-eabi-size"} {
					writeFixture(t, root, "bin/"+tool, toolDouble)
				}
				// Windows Make can directly execute a host .exe before trying a
				// script on PATH. Explicit Bash commands keep every tool isolated.
				command := `cd examples/L432_inst && make -B -j1 GCC_LISTINGS=0 C_SOURCES=probe.c ASM_SOURCES=startup.s TARGET=probe TRICE_FLAGS=-DCONFIGURATION=98 \
  'CLANG_CC=bash ../../bin/clang' 'CLANG_AS=bash ../../bin/clang -x assembler-with-cpp' \
  'GCC_CC=bash ../../bin/arm-none-eabi-gcc' 'GCC_AS=bash ../../bin/arm-none-eabi-gcc -x assembler-with-cpp' \
  'GCC_CP=bash ../../bin/arm-none-eabi-objcopy' 'GCC_SZ=bash ../../bin/arm-none-eabi-size' ` + target
				env := map[string]string{
					"LINK_FAIL": fail, "TRICE_BIND_INCLUDE_DIR": "generated headers", "CLANG_GCC_INCLUDE": "GCC headers",
					"CLANG_LIBC_INCLUDE": "Newlib headers", "CLANG_SYS_INCLUDES": "-isystem 'extra headers'", "CLANG_GCC_TOOLCHAIN": "",
				}
				out, err := runFixture(t, root, command, env)
				assert.Equal(t, fail != "0", err != nil, out)
				assert.Contains(t, out, "fixture linker diagnostic", "linker errors/warnings must be visible")
				linkArgs, readErr := os.ReadFile(filepath.Join(root, "examples/L432_inst/out."+target+"/probe.elf.args"))
				assert.NoError(t, readErr)
				assert.Contains(t, string(linkArgs), "-Wl,--no-warn-execstack\n")
				args, readErr := os.ReadFile(filepath.Join(root, "examples/L432_inst/out."+target+"/probe.o.args"))
				assert.NoError(t, readErr)
				assert.Contains(t, string(args), "-O3\n")
				assert.Contains(t, string(args), "-mcpu=cortex-m4\n")
				assert.Contains(t, string(args), "-mfloat-abi=hard\n")
				assert.Contains(t, string(args), "-ICore/Inc\n-Igenerated headers\n")
				assert.True(t, strings.Index(string(args), "-Igenerated headers\n") < strings.Index(string(args), "-I../../src\n"), string(args))
				assert.Contains(t, string(args), "-I../../src\n-I../../src/default_conf\n")
				// Both language paths must omit unsupported Clang switches while
				// retaining GCC-only options and the remaining enabled warnings.
				for _, object := range []string{"probe", "startup"} {
					data, err := os.ReadFile(filepath.Join(root, "examples/L432_inst/out."+target+"/"+object+".o.args"))
					if !assert.NoError(t, err) {
						continue
					}
					options := "\n" + string(data)
					for _, option := range []string{
						"-funsafe-loop-optimizations", "-Wno-builtin-declaration-mismatch", "-Wstack-usage=400",
						"-Wunsafe-loop-optimizations", "-Wlogical-op", "-Wnormalized=nfc",
						"-Wno-discarded-qualifiers", "-Wno-suggest-attribute=noreturn",
					} {
						assert.Equal(t, target == "gcc", strings.Contains(options, "\n"+option+"\n"), object+": "+option)
					}
					assert.Contains(t, options, "\n-O3\n")
					assert.NotContains(t, options, "\n-Wmissing-noreturn\n")
					assert.NotContains(t, options, "\n-Wwrite-strings\n")
					assert.Contains(t, options, "\n-Wall\n")
					assert.Contains(t, options, "\n-Wextra\n")
					assert.NotContains(t, options, "\n-Wno-unknown-warning-option\n")
					assert.NotContains(t, options, "\n-Wno-ignored-optimization-argument\n")
				}
				if target == "clang" {
					assert.Contains(t, string(args), "--target=arm-none-eabi\n")
					assert.Contains(t, string(args), "-isystem\nGCC headers\n-isystem\nNewlib headers\n-isystem\nextra headers\n")
					log, err := os.ReadFile(filepath.Join(root, "examples/L432_inst/gccLinksClangWarning.txt"))
					assert.NoError(t, err)
					assert.Contains(t, string(log), "fixture linker diagnostic")
				}
				if target == "clang" && fail == "0" {
					// Real Make must succeed with an existing directory, both for
					// the same configuration and after a configuration change.
					// Keep a sentinel to catch a workaround that deletes outputs.
					writeFixture(t, root, "examples/L432_inst/out.clang/keep.txt", "preserve existing output\n")
					for _, configuration := range []string{"98", "1"} {
						warmCommand := strings.Replace(command, "CONFIGURATION=98", "CONFIGURATION="+configuration, 1)
						out, err := runFixture(t, root, warmCommand, env)
						assert.NoError(t, err, out)
						assert.NotContains(t, out, "File exists")
						args, err := os.ReadFile(filepath.Join(root, "examples/L432_inst/out.clang/probe.o.args"))
						assert.NoError(t, err)
						assert.Contains(t, string(args), "-DCONFIGURATION="+configuration+"\n")
					}
					for _, object := range []string{"probe", "startup"} {
						calls, err := os.ReadFile(filepath.Join(root, "examples/L432_inst/out.clang/"+object+".o.calls"))
						assert.NoError(t, err)
						assert.Equal(t, 3, strings.Count(string(calls), "\n"), "each invocation must recompile "+object)
					}
					sentinel, err := os.ReadFile(filepath.Join(root, "examples/L432_inst/out.clang/keep.txt"))
					assert.NoError(t, err)
					assert.Equal(t, "preserve existing output\n", string(sentinel))
					for _, extension := range []string{"elf", "hex", "bin"} {
						image, err := os.ReadFile(filepath.Join(root, "examples/L432_inst/out.clang/probe."+extension))
						assert.NoError(t, err)
						assert.NotEmpty(t, image)
					}
				}
			})
		}
	}
}
