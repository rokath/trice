// SPDX-License-Identifier: MIT

package scripts_test

import (
	"bytes"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestBcSimLockFailures runs the real C implementation with operation-specific
// faults. The Windows adapter double checks error capture, not Windows races.
func TestBcSimLockFailures(t *testing.T) {
	for _, compiler := range []string{"gcc", "clang"} {
		t.Run(compiler, func(t *testing.T) {
			cc, err := exec.LookPath(compiler)
			require.NoError(t, err, "native bus tests require the existing C compilers")
			adapters := []string{"native"}
			if runtime.GOOS != "windows" {
				adapters = append(adapters, "windows_adapter_double")
			}
			for _, adapter := range adapters {
				t.Run(adapter, func(t *testing.T) {
					root := scriptFixture(t, "scripts/testdata/bcsim_lock_probe.c", "examples/TriceAbc/BcSim/BcSim.c", "examples/TriceAbc/BcSim/BcSim.h", "examples/TriceAbc/BcSim/BcSim_config.h")
					binary := filepath.Join(root, "probe")
					if runtime.GOOS == "windows" {
						binary += ".exe"
					}
					args := []string{"-std=c99", "-Wall", "-Wextra", "-Werror", "-pedantic"}
					if adapter == "windows_adapter_double" {
						writeFixture(t, root, "headers/direct.h", "/* The probe supplies the directory adapters. */\n")
						writeFixture(t, root, "headers/windows.h", `/* Minimal declarations for testing capture of Windows error state. */
#ifndef BCSIM_TEST_WINDOWS_H
#define BCSIM_TEST_WINDOWS_H
#include <stdint.h>
typedef uint32_t DWORD;
#define WINAPI
#define ERROR_SUCCESS 0u
extern unsigned long bcSimProbeDosError;
#define _doserrno bcSimProbeDosError
DWORD GetLastError(void);
void SetLastError(DWORD value);
DWORD GetCurrentProcessId(void);
void Sleep(DWORD ms);
#endif
`)
						args = append(args, "-D_WIN32", "-DBCSIM_TEST_WINDOWS_SHIM", "-Iheaders")
					}
					args = append(args, "scripts/testdata/bcsim_lock_probe.c", "-o", binary)
					build := exec.Command(cc, args...)
					build.Dir = root
					out, err := build.CombinedOutput()
					require.NoError(t, err, "%s", out)
					for _, scenario := range []string{"success", "contention", "timeout", "mkdir_error", "unlock_error", "header_and_unlock_error", "write_error"} {
						t.Run(scenario, func(t *testing.T) {
							// Separate run directories keep bus files and executables
							// independent, including Windows executable lifetimes.
							work := t.TempDir()
							var stdout, stderr bytes.Buffer
							command := exec.Command(binary, scenario)
							command.Dir, command.Stdout, command.Stderr = work, &stdout, &stderr
							err := command.Run()
							require.NoError(t, err, "stdout: %s\nstderr: %s", stdout.String(), stderr.String())
							failedOperation := ""
							switch scenario {
							case "mkdir_error", "write_error":
								failedOperation = "mkdir"
							case "unlock_error", "header_and_unlock_error":
								failedOperation = "rmdir"
							}
							if failedOperation == "" {
								assert.Empty(t, stderr.String(), "successful or ordinary contended locking stays quiet")
							} else {
								assert.Contains(t, stderr.String(), "operation="+failedOperation)
								assert.Contains(t, stderr.String(), `lock="bc.bus.lock"`)
								assert.Contains(t, stderr.String(), `participant="probe participant"`)
								assert.Contains(t, stderr.String(), "pid=4242")
								assert.Contains(t, stderr.String(), "errno=")
								assert.Contains(t, stderr.String(), "compiler=")
								if adapter == "windows_adapter_double" || runtime.GOOS == "windows" {
									assert.Contains(t, stderr.String(), "win32_error=")
									assert.Contains(t, stderr.String(), "crt_os_error=")
								}
							}
							if scenario == "mkdir_error" {
								assert.Contains(t, stdout.String(), "mkdir_calls=1", "unexpected mkdir failures are not retried")
							}
							if scenario == "success" || scenario == "contention" {
								data, err := os.ReadFile(filepath.Join(work, "bc.bus"))
								require.NoError(t, err)
								assert.Equal(t, []byte{0x41, 0x00, 0xff}, data, "diagnostics never enter the binary bus")
								assert.NoDirExists(t, filepath.Join(work, "bc.bus.lock"))
							}
						})
					}
				})
			}
		})
	}
}

// TestBcSimDemoParticipantFailures proves that every child contributes its exit
// status and that the controller reaps all children on failure or interruption.
func TestBcSimDemoParticipantFailures(t *testing.T) {
	for _, outcome := range []string{"success", "A", "B", "C", "D", "interrupt"} {
		t.Run(outcome, func(t *testing.T) {
			root := scriptFixture(t, "examples/TriceAbc/BcSimChk/demo.sh")
			writeFixture(t, root, "bin/uname", "#!/bin/sh\necho Linux\n")
			writeFixture(t, root, "examples/TriceAbc/BcSimChk/build.sh", "#!/bin/sh\nmkdir -p build events/active\n")
			writeFixture(t, root, "examples/TriceAbc/BcSimChk/build/bcSimDevice", `#!/usr/bin/env bash
set -eu
name=$2
touch "events/active/$name"
trap 'rm -f "events/active/$name"' EXIT
trap 'exit 143' TERM
trap 'exit 130' INT
touch bc.bus bc.log
printf '%s\n' "$name" >> events/started
if [ "$OUTCOME" = interrupt ]; then
  touch "events/$name.ready"
  if [ "$name" = D ]; then
    for peer in A B C D; do until [ -f "events/$peer.ready" ]; do sleep 0.01; done; done
    kill -TERM "$PPID"
  fi
  # Bound a broken controller's children so a regression cannot hang the suite.
  guard=0
  while [ "$guard" -lt 500 ]; do sleep 0.01; guard=$((guard + 1)); done
  touch events/guard_expired
  exit 91
fi
if [ "$name" = "$OUTCOME" ]; then exit 37; fi
printf '%s\n' "$name" >> events/finished
`)
			out, err := runFixture(t, root, "bash examples/TriceAbc/BcSimChk/demo.sh", map[string]string{"OUTCOME": outcome})
			if outcome == "success" {
				assert.NoError(t, err, out)
			} else {
				assert.Error(t, err, out)
				if outcome != "interrupt" {
					assert.Contains(t, out, "error: BcSim participant "+outcome+" exited with status 37")
				}
			}
			active, err := os.ReadDir(filepath.Join(root, "examples/TriceAbc/BcSimChk/events/active"))
			require.NoError(t, err)
			assert.Empty(t, active, "no participant survives its controller")
			assert.NoFileExists(t, filepath.Join(root, "examples/TriceAbc/BcSimChk/events/guard_expired"), "the controller must stop its children before the fixture safety limit")
			started, err := os.ReadFile(filepath.Join(root, "examples/TriceAbc/BcSimChk/events/started"))
			require.NoError(t, err)
			assert.Len(t, strings.Fields(string(started)), 4, "all four participants were exercised")
		})
	}
}
