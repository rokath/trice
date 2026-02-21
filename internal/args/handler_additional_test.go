// SPDX-License-Identifier: MIT

package args

import (
	"bytes"
	"testing"

	"github.com/tj/assert"
)

type versionState struct {
	Version   string
	Commit    string
	Date      string
	Branch    string
	GitState  string
	GitStatus string
	BuiltBy   string
	Verbose   bool
}

func snapshotVersionState() versionState {
	return versionState{
		Version:   Version,
		Commit:    Commit,
		Date:      Date,
		Branch:    Branch,
		GitState:  GitState,
		GitStatus: GitStatus,
		BuiltBy:   BuiltBy,
		Verbose:   Verbose,
	}
}

func restoreVersionState(s versionState) {
	Version = s.Version
	Commit = s.Commit
	Date = s.Date
	Branch = s.Branch
	GitState = s.GitState
	GitStatus = s.GitStatus
	BuiltBy = s.BuiltBy
	Verbose = s.Verbose
}

func TestScVersionDevFallback(t *testing.T) {
	old := snapshotVersionState()
	defer restoreVersionState(old)

	Version = ""
	Commit = ""
	Date = ""
	Branch = ""
	GitState = ""
	GitStatus = ""
	BuiltBy = ""
	Verbose = false

	var out bytes.Buffer
	err := scVersion(&out)
	assert.Nil(t, err)
	assert.Equal(t, "version=dev (no build info)\n", out.String())
}

func TestScVersionReleaseOutput(t *testing.T) {
	old := snapshotVersionState()
	defer restoreVersionState(old)

	Version = "1.2.3"
	Commit = "abc1234"
	Date = "2026-02-21T12:00:00Z"
	Branch = ""
	GitState = "clean"
	GitStatus = ""
	BuiltBy = "ci"
	Verbose = false

	var out bytes.Buffer
	err := scVersion(&out)
	assert.Nil(t, err)
	assert.Equal(t, "version=1.2.3, commit=abc1234, built at 2026-02-21T12:00:00Z (built by ci)\n", out.String())
}

func TestScVersionBranchFallbackAndDirtyList(t *testing.T) {
	old := snapshotVersionState()
	defer restoreVersionState(old)

	Version = ""
	Commit = "abc1234"
	Date = "2026-02-21T12:00:00Z"
	Branch = "main"
	GitState = "dirty"
	GitStatus = "M internal/args/handler.go|?? internal/args/new_test.go|"
	BuiltBy = ""
	Verbose = true

	var out bytes.Buffer
	err := scVersion(&out)
	assert.Nil(t, err)
	s := out.String()
	assert.Contains(t, s, "https://github.com/rokath/trice\n")
	assert.Contains(t, s, "branch=main (local modifications at build time), commit=abc1234, built at 2026-02-21T12:00:00Z\n")
	assert.Contains(t, s, "modified files at build time:\n")
	assert.Contains(t, s, "  M internal/args/handler.go\n")
	assert.Contains(t, s, "  ?? internal/args/new_test.go\n")
}

func TestIsLogFlagPassed(t *testing.T) {
	FlagsInit()
	err := fsScLog.Parse([]string{"-ts32", "ms", "-encoding", "CHAR"})
	assert.Nil(t, err)
	assert.True(t, isLogFlagPassed("ts32"))
	assert.True(t, isLogFlagPassed("encoding"))
	assert.False(t, isLogFlagPassed("ts16"))
}
