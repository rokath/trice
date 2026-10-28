// SPDX-License-Identifier: MIT

package main

import (
	"archive/zip"
	"bytes"
	"encoding/json"
	"fmt"
	"net/http"
	"net/http/httptest"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strings"
	"sync/atomic"
	"testing"
	"time"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

// TestGoTestEvidenceKeepsDiagnosticsSeparate exercises the real shell capture
// and audit executable, including a genuine cold module download from a local
// proxy. No published dependency or external network service is needed.
func TestGoTestEvidenceKeepsDiagnosticsSeparate(t *testing.T) {
	bash, err := exec.LookPath("bash")
	require.NoError(t, err, "native shell evidence checks require Bash")
	realGo := filepath.Join(runtime.GOROOT(), "bin", "go")
	auditor := filepath.Join(t.TempDir(), "testcheck")
	if runtime.GOOS == "windows" {
		realGo += ".exe"
		auditor += ".exe"
	}
	output, err := exec.Command(realGo, "build", "-o", auditor, ".").CombinedOutput()
	require.NoError(t, err, "%s", output)
	// Reuse compiled standard packages; only the module cache must start empty.
	cacheCommand := exec.Command(realGo, "env", "GOCACHE")
	cacheCommand.Env = append(os.Environ(), "GOTOOLCHAIN=local")
	buildCache, err := cacheCommand.Output()
	require.NoError(t, err)
	// Copy the production helper without the simplified orchestration fixture
	// override, so stdout/stderr handling and strict auditing are both real.
	newFixture := func(t *testing.T) string {
		t.Helper()
		root := t.TempDir()
		require.NoError(t, os.Mkdir(filepath.Join(root, "scripts"), 0o755))
		for _, name := range []string{"_100_test_common.sh", "_105_setup_test_tools.sh", "test-tools.env"} {
			data, err := os.ReadFile(filepath.Join("..", name))
			require.NoError(t, err)
			require.NoError(t, os.WriteFile(filepath.Join(root, "scripts", name), data, 0o644))
		}
		return root
	}
	// Explicit environments keep outer suite selections and caches outside the
	// fixture. os/exec uses the final value when an inherited key is repeated.
	run := func(root, body string, environment ...string) (string, error) {
		command := exec.Command(bash, "-c", "source scripts/_100_test_common.sh\nLOGFILE=\"$LOG_DIR/evidence.log\"\n"+body)
		command.Dir = root
		command.Env = append(os.Environ(),
			"LOG_DIR="+filepath.Join(root, "log"), "TRICE_TMP_DIR="+filepath.Join(root, "temp"),
			"TRICE_BIN_DIR="+filepath.Join(root, "bin"), "TRICE_TEST_REPORT_BIN="+auditor,
			"TRICE_TEST_AUTO_SETUP=0", "TRICE_TEST_FIXED_HOST=1", "SELECTED=quick",
			"GOTOOLCHAIN=local", "GOWORK=off", "GOFLAGS=", "GOCACHE="+strings.TrimSpace(string(buildCache)),
			"TRICE_REQUIRED_TESTS=TestEvidence")
		command.Env = append(command.Env, environment...)
		output, err := command.CombinedOutput()
		return string(output), err
	}
	passed := "{\"Action\":\"start\",\"Package\":\"example\"}\n" +
		"{\"Action\":\"pass\",\"Package\":\"example\",\"Test\":\"TestEvidence\"}\n" +
		"{\"Action\":\"pass\",\"Package\":\"example\"}\n"
	for _, tc := range []struct {
		name, events, required string
		goStatus, want         int
	}{
		{"passed_with_diagnostics", passed, "TestEvidence", 0, 0},
		{"failed_test", strings.ReplaceAll(passed, `"pass"`, `"fail"`), "TestEvidence", 1, 1},
		{"nonzero_go_status", passed, "TestEvidence", 7, 1},
		{"missing_required_test", passed, "TestAbsent", 0, 2},
		{"unexpected_skip", strings.Replace(passed, `"pass","Package":"example","Test"`, `"skip","Package":"example","Test"`, 1), "TestEvidence", 0, 2},
		{"empty_stdout", "", "TestEvidence", 1, 2},
		{"non_JSON_stdout", "go: unexpected stdout\n" + passed, "TestEvidence", 0, 2},
	} {
		for _, quiet := range []int{0, 1} {
			t.Run(fmt.Sprintf("%s/quiet_%d", tc.name, quiet), func(t *testing.T) {
				root := newFixture(t)
				diagnostic := "go: downloading example.com/evidencevalue v1.0.0\ncompiler diagnostic\n"
				body := `QUIET="$EVIDENCE_QUIET"
# Model the go subprocess while retaining the production capture and auditor.
go() {
  [ "$1" = test ] && [ "$2" = -json ] || return 90
  printf '%s' "$EVIDENCE_DIAGNOSTIC" >&2
  printf '%s' "$EVIDENCE_EVENTS"
  return "$EVIDENCE_GO_STATUS"
}
run_go_tests ./example
`
				out, err := run(root, body, "EVIDENCE_EVENTS="+tc.events, "EVIDENCE_DIAGNOSTIC="+diagnostic,
					fmt.Sprintf("EVIDENCE_GO_STATUS=%d", tc.goStatus), fmt.Sprintf("EVIDENCE_QUIET=%d", quiet),
					"TRICE_REQUIRED_TESTS="+tc.required)
				if tc.want == 0 {
					assert.NoError(t, err, out)
				} else {
					var exit *exec.ExitError
					require.ErrorAs(t, err, &exit, out)
					assert.Equal(t, tc.want, exit.ExitCode(), out)
				}
				files, err := filepath.Glob(filepath.Join(root, "log", "evidence.log.go.*"))
				require.NoError(t, err)
				require.Len(t, files, 2, "retain both the original JSON and separate stderr")
				for _, file := range files {
					data, err := os.ReadFile(file)
					require.NoError(t, err)
					if strings.HasSuffix(file, ".stderr") {
						assert.Equal(t, diagnostic, string(data))
					} else {
						assert.Equal(t, tc.events, string(data), "never filter malformed stdout into a false pass")
					}
				}
				log, err := os.ReadFile(filepath.Join(root, "log", "evidence.log"))
				require.NoError(t, err)
				assert.Contains(t, string(log), diagnostic)
				if quiet == 0 {
					assert.Contains(t, out, diagnostic)
				} else {
					assert.Empty(t, out)
				}
			})
		}
	}
	t.Run("real_cold_and_warm_module_cache", func(t *testing.T) {
		root := newFixture(t)
		module := "module example.com/evidencevalue\ngo 1.20\n"
		var archive bytes.Buffer
		writer := zip.NewWriter(&archive)
		for name, contents := range map[string]string{
			"go.mod":   module,
			"value.go": "// SPDX-License-Identifier: MIT\npackage value\n// Text supplies the value checked by the consumer.\nconst Text = \"downloaded fixture\"\n",
		} {
			entry, err := writer.Create("example.com/evidencevalue@v1.0.0/" + name)
			require.NoError(t, err)
			_, err = entry.Write([]byte(contents))
			require.NoError(t, err)
		}
		require.NoError(t, writer.Close())
		responses := map[string][]byte{
			"/example.com/evidencevalue/@v/v1.0.0.mod":  []byte(module),
			"/example.com/evidencevalue/@v/v1.0.0.info": []byte(`{"Version":"v1.0.0","Time":"2020-01-01T00:00:00Z"}`),
			"/example.com/evidencevalue/@v/v1.0.0.zip":  archive.Bytes(),
		}
		var requests atomic.Int32
		server := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
			requests.Add(1)
			if data, ok := responses[r.URL.Path]; ok {
				_, _ = w.Write(data)
			} else {
				http.NotFound(w, r)
			}
		}))
		defer server.Close()
		require.NoError(t, os.WriteFile(filepath.Join(root, "go.mod"), []byte("module example.com/evidence\ngo 1.20\nrequire example.com/evidencevalue v1.0.0\n"), 0o644))
		require.NoError(t, os.WriteFile(filepath.Join(root, "evidence_test.go"), []byte(`// SPDX-License-Identifier: MIT
package evidence_test
import ("testing"; value "example.com/evidencevalue")
// TestEvidence requires the locally served module to execute successfully.
func TestEvidence(t *testing.T) {
    if value.Text != "downloaded fixture" { t.Fatal(value.Text) }
}
`), 0o644))
		var coldRequests int32
		for _, state := range []string{"cold", "warm"} {
			logDir := filepath.Join(root, "log", state)
			// Keep only this disposable module cache writable for TempDir cleanup.
			out, err := run(root, "QUIET=1\ngo() { \"$EVIDENCE_GO\" \"$@\"; }\nrun_go_tests -mod=mod -modcacherw -count=1 .\n",
				"EVIDENCE_GO="+realGo, "LOG_DIR="+logDir, "GOMODCACHE="+filepath.Join(root, "modules"),
				"GOPROXY="+server.URL, "GOSUMDB=off", "GONOPROXY=", "GONOSUMDB=", "GOPRIVATE=")
			require.NoError(t, err, "%s: %s", state, out)
			files, err := filepath.Glob(filepath.Join(logDir, "evidence.log.go.*"))
			require.NoError(t, err)
			require.Len(t, files, 2)
			for _, file := range files {
				data, err := os.ReadFile(file)
				require.NoError(t, err)
				if strings.HasSuffix(file, ".stderr") {
					if state == "cold" {
						assert.Contains(t, string(data), "go: downloading example.com/evidencevalue v1.0.0")
					} else {
						assert.Empty(t, string(data))
					}
				} else {
					var audited bytes.Buffer
					assert.Zero(t, audit(bytes.NewReader(data), &audited, runtime.GOOS, []string{"TestEvidence"}), audited.String())
				}
			}
			if state == "cold" {
				coldRequests = requests.Load()
				assert.Positive(t, coldRequests)
			} else {
				assert.Equal(t, coldRequests, requests.Load(), "warm run must reuse the module without HTTP requests")
			}
		}
	})
}

// TestAuditRequiresExecutedEvidence guards against false green results from
// empty selections, package failures, partial streams and unexpected skips.
func TestAuditRequiresExecutedEvidence(t *testing.T) {
	for _, tc := range []struct {
		name, platform, action, reason string
		required                       []string
		want                           int
	}{
		{"passed", "linux", "pass", "", []string{"TestExample"}, 0},
		{"missing_selected_test", "linux", "pass", "", []string{"TestMissing"}, 2},
		{"missing_compiler", "linux", "skip", "no C compiler available", nil, 2},
		{"test_failure", "windows", "fail", "incorrect decoded record", nil, 1},
		{"required_test_failure", "linux", "fail", "incorrect decoded record", []string{"TestExample"}, 1},
		{"disabled_windows_TCP", "windows", "skip", "TCP requires opt-in", nil, 2},
	} {
		t.Run(tc.name, func(t *testing.T) {
			var input, output bytes.Buffer
			encoder := json.NewEncoder(&input)
			for _, event := range []testEvent{
				{Action: "start", Package: "example"},
				{Action: "output", Package: "example", Test: "TestExample", Output: tc.reason},
				{Action: tc.action, Package: "example", Test: "TestExample"},
				{Action: "pass", Package: "example"},
			} {
				assert.NoError(t, encoder.Encode(event))
			}
			assert.Equal(t, tc.want, audit(&input, &output, tc.platform, tc.required), output.String())
		})
	}
	for _, input := range []string{"", "{", `{"Action":"start","Package":"example"}`} {
		assert.Equal(t, 2, audit(strings.NewReader(input), &bytes.Buffer{}, "linux", nil))
	}
	var output bytes.Buffer
	input := "{\"Action\":\"build-output\",\"ImportPath\":\"example\",\"Output\":\"compiler: deliberate error\\n\"}\n{\"Action\":\"build-fail\",\"ImportPath\":\"example\"}\n"
	assert.Equal(t, 1, audit(strings.NewReader(input), &output, "linux", nil))
	assert.Contains(t, output.String(), "compiler: deliberate error")
}

// TestSkipExceptionsStayNarrow prevents capability exceptions from suppressing
// a missing compiler, a newly skipped test or a Windows shell regression.
func TestSkipExceptionsStayNarrow(t *testing.T) {
	assert.True(t, allowedSkip("windows", "TestHandleSIGTERMExitsAndCloses", "syscall.SIGTERM subprocess signaling is not supported on Windows"))
	assert.False(t, allowedSkip("linux", "TestHandleSIGTERMExitsAndCloses", "SIGTERM subprocess"))
	assert.False(t, allowedSkip("windows", "TestHandleSIGTERMExitsAndCloses", "compiler missing"))
	assert.True(t, allowedSkip("linux", "TestStampShellQuoting/cmd_interactive", "cmd.exe is not installed"))
	assert.False(t, allowedSkip("windows", "TestStampShellQuoting/cmd_interactive", "cmd.exe is not installed"))
	assert.False(t, allowedSkip("linux", "TestStampShellQuoting/Bash_and_Git_Bash", "bash is not installed"))
}

// TestReportRetainsHostAndStepEvidence checks the published contract rather
// than its JSON formatting. Invalid records must never replace a prior report.
func TestReportRetainsHostAndStepEvidence(t *testing.T) {
	dir := t.TempDir()
	require.NoError(t, os.WriteFile(filepath.Join(dir, "steps.tsv"), []byte("compiler\tPASS\t3\tcompiler.log\npaths\tINCOMPLETE\t1\tpaths.log\n"), 0o644))
	require.NoError(t, os.WriteFile(filepath.Join(dir, "tools.log"), []byte("Selected native GCC and SDK\n"), 0o644))
	started := time.Now().Add(-5 * time.Second).Unix()
	require.NoError(t, writeReport(dir, "quick", "INCOMPLETE", started))
	data, err := os.ReadFile(filepath.Join(dir, "results.json"))
	require.NoError(t, err)
	var report suiteReport
	require.NoError(t, json.Unmarshal(data, &report))
	assert.Equal(t, "INCOMPLETE", report.Result)
	assert.Equal(t, "quick", report.Selection)
	assert.NotEmpty(t, report.Commit)
	assert.NotEmpty(t, report.OS)
	assert.NotEmpty(t, report.Arch)
	assert.Equal(t, "Selected native GCC and SDK\n", report.Context)
	assert.Equal(t, []stepResult{{"compiler", "PASS", 3, "compiler.log"}, {"paths", "INCOMPLETE", 1, "paths.log"}}, report.Steps)
	assert.GreaterOrEqual(t, report.Duration, int64(5))
	for _, invalid := range []string{"broken\n", "compiler\tPASS\t-1\tcompiler.log\n"} {
		require.NoError(t, os.WriteFile(filepath.Join(dir, "steps.tsv"), []byte(invalid), 0o644))
		assert.Error(t, writeReport(dir, "quick", "PASS", started))
		retained, readErr := os.ReadFile(filepath.Join(dir, "results.json"))
		require.NoError(t, readErr)
		assert.Equal(t, data, retained)
	}
}
