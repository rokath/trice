// SPDX-License-Identifier: MIT

package args

import (
	"bytes"
	"flag"
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

// snapshotVersionState captures version-related global state so tests can restore it afterwards.
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

// restoreVersionState restores version-related global state from a saved snapshot.
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

// TestScVersionDevFallback verifies the expected behavior.
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

// TestScVersionReleaseOutput verifies the expected behavior.
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

// TestScVersionBranchFallbackAndDirtyList verifies the expected behavior.
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

// TestIsLogFlagPassed verifies the expected behavior.
func TestIsLogFlagPassed(t *testing.T) {
	FlagsInit()
	err := fsScLog.Parse([]string{"-ts32", "ms", "-encoding", "CHAR"})
	assert.Nil(t, err)
	assert.True(t, isLogFlagPassed("ts32"))
	assert.True(t, isLogFlagPassed("encoding"))
	assert.False(t, isLogFlagPassed("ts16"))
}

// TestVisFlagIsRepeatableAndResetWithLogFlags verifies CLI collection without leaking rules across parses.
func TestVisFlagIsRepeatableAndResetWithLogFlags(t *testing.T) {
	FlagsInit()
	err := fsScLog.Parse([]string{
		"-vis", `msg:printf("%d",v0)@one.txt`,
		"-vis", `imu:printf("%f",v0)@two.txt`,
	})
	assert.Nil(t, err)
	assert.Equal(t, []string{
		`msg:printf("%d",v0)@one.txt`,
		`imu:printf("%f",v0)@two.txt`,
	}, []string(visRules))
	assert.True(t, isLogFlagPassed("vis"))

	FlagsInit()
	assert.Empty(t, visRules)
	assert.False(t, isLogFlagPassed("vis"))
}

// TestInfoHelpersWriteText verifies the expected behavior.
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
		{"bindIDsInfo", bindIDsInfo, "sub-command 'b|bind'"},
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

// TestScHelpWithSelectedSections verifies the expected behavior.
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

// TestHandlerVersionSubcommands verifies the expected behavior.
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
	for _, cmd := range []string{"ver", "version", "--version", "-version"} {
		var out bytes.Buffer
		err := Handler(&out, fSys, []string{"trice", cmd})
		assert.Nil(t, err)
		assert.True(t, strings.Contains(out.String(), "version=1.0.0"))
	}
}

// TestHandlerGenerateWithoutParameters verifies the expected behavior.
func TestHandlerGenerateWithoutParameters(t *testing.T) {
	FlagsInit()
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	var out bytes.Buffer
	err := Handler(&out, fSys, []string{"trice", "generate"})
	assert.Nil(t, err)
	assert.Contains(t, out.String(), `The "trice generate" command needs at least one parameter.`)
}

// TestNormalizeGenerateLogCPath verifies both documented optional-path forms
// without retaining the removed -tilC compatibility spelling.
func TestNormalizeGenerateLogCPath(t *testing.T) {
	assert.Equal(t, []string{"-logC=build/til.c", "-src", "src"}, normalizeGenerateArgs([]string{"-logC", "build/til.c", "-src", "src"}))
	assert.Equal(t, []string{"-logC=build/til.c"}, normalizeGenerateArgs([]string{"-logC=build/til.c"}))
	assert.Equal(t, []string{"-tilC", "old.c"}, normalizeGenerateArgs([]string{"-tilC", "old.c"}))
}

// TestHandlerAddInsertCleanOnMissingSource verifies the expected behavior.
func TestHandlerAddInsertCleanOnMissingSource(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	assert.Nil(t, fSys.WriteFile("til.json", []byte("{}"), 0o644))
	assert.Nil(t, fSys.WriteFile("li.json", []byte("{}"), 0o644))
	id.FnJSON = "til.json"
	id.LIFnJSON = "li.json"
	tests := [][]string{
		{"trice", "add", "-src", "missing-source-tree"},
		{"trice", "insert", "-src", "missing-source-tree"},
		{"trice", "bind", "-src", "missing-source-tree"},
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

// TestLogLocationPathFlags verifies the log-specific directory display limit.
func TestLogLocationPathFlags(t *testing.T) {
	oldMaxDirs := id.LIMaxDirs
	t.Cleanup(func() {
		id.LIMaxDirs = oldMaxDirs
	})

	FlagsInit()
	assert.Nil(t, fsScLog.Lookup("liPath"))
	assert.Nil(t, fsScLog.Lookup("liRoot"))
	assert.Equal(t, "0", fsScLog.Lookup("liMaxDirs").DefValue)
	assert.NoError(t, fsScLog.Parse([]string{"-liMaxDirs", "3"}))
	assert.Equal(t, 3, id.LIMaxDirs)
}

// TestGenerationLocationRootFlags verifies the shared root flag on all ID commands.
func TestGenerationLocationRootFlags(t *testing.T) {
	oldRoot := id.LIRoot
	t.Cleanup(func() { id.LIRoot = oldRoot })

	FlagsInit()
	for _, flagSet := range []*flag.FlagSet{fsScAdd, fsScInsert, fsScBind, fsScClean} {
		assert.Nil(t, flagSet.Lookup("liPath"))
		assert.Equal(t, "", flagSet.Lookup("liRoot").DefValue)
	}
	assert.NoError(t, fsScInsert.Parse([]string{"-liRoot", "project"}))
	assert.Equal(t, "project", id.LIRoot)
}

// TestHandlerBindGeneratesSidecar exercises the public command and bind-specific directory flag.
func TestHandlerBindGeneratesSidecar(t *testing.T) {
	FlagsInit()
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	assert.NoError(t, fSys.MkdirAll("project", 0o755))
	assert.NoError(t, fSys.WriteFile("til.json", []byte("{}\n"), 0o644))
	assert.NoError(t, fSys.WriteFile("li.json", []byte("{}\n"), 0o644))
	const source = "trice(\"msg:handler=%d\\n\", 1);\n"
	assert.NoError(t, fSys.WriteFile("project/module.c", []byte(source), 0o644))

	var output bytes.Buffer
	err := Handler(&output, fSys, []string{
		"trice", "bind",
		"-src", "project/module.c",
		"-til", "til.json",
		"-li", "li.json",
		"-bindDir", "generated/triceIDs",
		"-IDMin", "100",
		"-IDMax", "199",
		"-IDMethod", "upward",
		"-defaultStampSize", "16",
	})
	assert.NoError(t, err)
	bound, readErr := fSys.ReadFile("project/module.c")
	assert.NoError(t, readErr)
	assert.Contains(t, string(bound), `#include "trice_module_c_K`)
	entries, readErr := fSys.ReadDir("generated/triceIDs")
	assert.NoError(t, readErr)
	assert.Len(t, entries, 1)
}

// TestHandlerBindImplicitSourceHint verifies that a failed recursive default
// scan explains its scope, while the same failure under an explicit source
// selection does not add the hint.
func TestHandlerBindImplicitSourceHint(t *testing.T) {
	oldSrcs := append(id.ArrayFlag(nil), id.Srcs...)
	oldLogfileName := LogfileName
	t.Cleanup(func() {
		id.Srcs = oldSrcs
		LogfileName = oldLogfileName
		FlagsInit()
	})
	LogfileName = "off"

	for _, test := range []struct {
		name       string
		sourceArgs []string
		wantHint   bool
	}{
		{name: "implicit recursive root", wantHint: true},
		{name: "explicit source", sourceArgs: []string{"-src", "broken.c"}, wantHint: false},
	} {
		t.Run(test.name, func(t *testing.T) {
			id.Srcs = nil
			FlagsInit()
			fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
			assert.NoError(t, fSys.WriteFile("til.json", []byte("{}\n"), 0o644))
			assert.NoError(t, fSys.WriteFile("li.json", []byte("{}\n"), 0o644))
			assert.NoError(t, fSys.WriteFile("broken.c", []byte("trice(value);\n"), 0o644))

			args := []string{"trice", "bind", "-til", "til.json", "-li", "li.json"}
			args = append(args, test.sourceArgs...)
			var output bytes.Buffer
			err := Handler(&output, fSys, args)
			assert.Error(t, err)
			if test.wantHint {
				assert.Contains(t, output.String(), implicitBindSourceHint)
			} else {
				assert.NotContains(t, output.String(), implicitBindSourceHint)
			}
		})
	}
}

// TestHandlerBindHelp verifies that both public names and both standard help flags exit cleanly.
func TestHandlerBindHelp(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	for _, subcommand := range []string{"bind", "b"} {
		for _, helpFlag := range []string{"-h", "--help"} {
			FlagsInit()
			var output bytes.Buffer
			err := Handler(&output, fSys, []string{"trice", subcommand, helpFlag})
			assert.NoError(t, err)
			assert.Contains(t, output.String(), "Usage of bind:")
			assert.NotContains(t, output.String(), "flag: help requested")
		}
	}
}
