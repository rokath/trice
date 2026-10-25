// SPDX-License-Identifier: MIT

package main

import (
	"bytes"
	"encoding/json"
	"os"
	"path/filepath"
	"strings"
	"testing"
	"time"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

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
