// SPDX-License-Identifier: MIT

package args

import (
	"bytes"
	"io"
	"strings"
	"testing"

	"github.com/rokath/trice/internal/id"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
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

func TestInfoHelpersWriteText(t *testing.T) {
	FlagsInit()
	tt := []struct {
		name   string
		fn     func(io.Writer) error
		marker string
	}{
		{"versionInfo", versionInfo, "sub-command 'ver|version'"},
		{"helpInfo", helpInfo, "sub-command 'h|help'"},
		{"logInfo", logInfo, "sub-command 'l|log'"},
		{"scanInfo", scanInfo, "sub-command 's|scan'"},
		{"displayServerInfo", displayServerInfo, "sub-command 'ds|displayServer'"},
		{"shutdownInfo", shutdownInfo, "sub-command 'sd|shutdown'"},
		{"insertIDsInfo", insertIDsInfo, "sub-command 'i|insert'"},
		{"cleanIDsInfo", cleanIDsInfo, "sub-command 'c|clean'"},
		{"addInfo", addInfo, "sub-command 'a|add'"},
		{"generateInfo", generateInfo, "sub-command 'g|gen|generate'"},
	}
	for _, tc := range tt {
		t.Run(tc.name, func(t *testing.T) {
			var out bytes.Buffer
			err := tc.fn(&out)
			assert.Nil(t, err)
			assert.Contains(t, out.String(), tc.marker)
		})
	}
}

func TestScHelpWithSelectedSections(t *testing.T) {
	FlagsInit()
	allHelp = false
	displayServerHelp = true
	helpHelp = true
	defer func() {
		allHelp = false
		displayServerHelp = false
		helpHelp = false
	}()

	var out bytes.Buffer
	err := scHelp(&out)
	assert.Nil(t, err)
	s := out.String()
	assert.Contains(t, s, "syntax: 'trice sub-command' [params]")
	assert.Contains(t, s, "sub-command 'ds|displayServer'")
	assert.Contains(t, s, "sub-command 'h|help'")
}

func TestHandlerVersionSubcommands(t *testing.T) {
	old := snapshotVersionState()
	defer restoreVersionState(old)

	FlagsInit()
	Version = "1.0.0"
	Commit = "abc1234"
	Date = "2026-02-21T12:00:00Z"
	Branch = ""
	GitState = "clean"
	GitStatus = ""
	BuiltBy = ""
	Verbose = false

	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	for _, cmd := range []string{"ver", "version"} {
		var out bytes.Buffer
		err := Handler(&out, fSys, []string{"trice", cmd})
		assert.Nil(t, err)
		assert.True(t, strings.Contains(out.String(), "version=1.0.0"))
	}
}

func TestHandlerGenerateWithoutParameters(t *testing.T) {
	FlagsInit()
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	var out bytes.Buffer
	err := Handler(&out, fSys, []string{"trice", "generate"})
	assert.Nil(t, err)
	assert.Contains(t, out.String(), `The "trice generate" command needs at least one parameter.`)
}

func TestHandlerAddInsertCleanOnMissingSource(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	assert.Nil(t, fSys.WriteFile("til.json", []byte("{}"), 0o644))
	assert.Nil(t, fSys.WriteFile("li.json", []byte("{}"), 0o644))
	id.FnJSON = "til.json"
	id.LIFnJSON = "li.json"
	tests := [][]string{
		{"trice", "add", "-src", "missing-source-tree"},
		{"trice", "insert", "-src", "missing-source-tree"},
		{"trice", "clean", "-src", "missing-source-tree"},
	}
	for _, args := range tests {
		FlagsInit() // reset global flag sets before each parse
		var out bytes.Buffer
		err := Handler(&out, fSys, args)
		assert.Nil(t, err)
		assert.Contains(t, out.String(), "missing-source-tree does not exist!")
	}
}
