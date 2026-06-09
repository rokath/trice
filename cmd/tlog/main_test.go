// SPDX-License-Identifier: MIT

package main

import (
	"bytes"
	"os"
	"testing"

	"github.com/rokath/trice/internal/args"
	"github.com/spf13/afero"
	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestDoitMapsArgsToLogCommand(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	args.FlagsInit()
	require.NoError(t, fSys.WriteFile("til.json", []byte("{}"), 0o644))

	previousArgs := os.Args
	previousVersion := version
	previousCommit := commit
	previousDate := date
	previousBranch := branch
	previousGitState := gitState
	previousGitStatus := gitStatus
	previousBuiltBy := builtBy
	t.Cleanup(func() {
		os.Args = previousArgs
		version = previousVersion
		commit = previousCommit
		date = previousDate
		branch = previousBranch
		gitState = previousGitState
		gitStatus = previousGitStatus
		builtBy = previousBuiltBy
	})

	version = "v1.2.3"
	commit = "abc1234"
	date = "2026-04-05T23:00:00+02:00"
	branch = "origin/tlog"
	gitState = "dirty"
	gitStatus = "M file.go"
	builtBy = "unit-test"
	os.Args = []string{
		"tlog",
		"-encoding", "DUMP",
		"-port", "DUMP",
		"-args", "41 42 43",
		"-dc", "8",
		"-li", "off",
		"-hs", "off",
		"-color", "none",
		"-prefix", "off",
		"-ts", "off",
	}

	var out bytes.Buffer
	require.NoError(t, doit(&out, fSys))

	assert.Equal(t, "41 42 43 \n", out.String())
	assert.Equal(t, version, args.Version)
	assert.Equal(t, commit, args.Commit)
	assert.Equal(t, date, args.Date)
	assert.Equal(t, branch, args.Branch)
	assert.Equal(t, gitState, args.GitState)
	assert.Equal(t, gitStatus, args.GitStatus)
	assert.Equal(t, builtBy, args.BuiltBy)
}

func TestDoitAcceptsVersionSwitch(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	previousArgs := os.Args
	previousVersion := version
	previousCommit := commit
	previousDate := date
	previousBranch := branch
	previousGitState := gitState
	previousGitStatus := gitStatus
	previousBuiltBy := builtBy
	t.Cleanup(func() {
		os.Args = previousArgs
		version = previousVersion
		commit = previousCommit
		date = previousDate
		branch = previousBranch
		gitState = previousGitState
		gitStatus = previousGitStatus
		builtBy = previousBuiltBy
	})

	version = "v1.2.3"
	commit = "abc1234"
	date = "2026-04-05T23:00:00+02:00"
	gitState = "clean"
	builtBy = "unit-test"

	for _, versionSwitch := range []string{"--version", "-version"} {
		t.Run(versionSwitch, func(t *testing.T) {
			args.FlagsInit()
			os.Args = []string{"tlog", versionSwitch}
			var out bytes.Buffer
			require.NoError(t, doit(&out, fSys))

			assert.Contains(t, out.String(), "version=v1.2.3")
			assert.Contains(t, out.String(), "commit=abc1234")
			assert.Contains(t, out.String(), "(built by unit-test)")
		})
	}
}

func TestDoitAcceptsHelpAndVersionWithoutTIL(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}

	previousArgs := os.Args
	previousVersion := version
	previousCommit := commit
	previousDate := date
	previousBranch := branch
	previousGitState := gitState
	previousGitStatus := gitStatus
	previousBuiltBy := builtBy
	t.Cleanup(func() {
		os.Args = previousArgs
		version = previousVersion
		commit = previousCommit
		date = previousDate
		branch = previousBranch
		gitState = previousGitState
		gitStatus = previousGitStatus
		builtBy = previousBuiltBy
	})

	version = "v1.2.3"
	commit = "abc1234"
	date = "2026-04-05T23:00:00+02:00"
	gitState = "clean"
	builtBy = "unit-test"

	for _, tc := range []struct {
		name     string
		osArgs   []string
		expected string
	}{
		{name: "version", osArgs: []string{"tlog", "version"}, expected: "version=v1.2.3"},
		{name: "ver", osArgs: []string{"tlog", "ver"}, expected: "version=v1.2.3"},
		{name: "help", osArgs: []string{"tlog", "help"}, expected: "sub-command 'l|log'"},
		{name: "h", osArgs: []string{"tlog", "h"}, expected: "sub-command 'l|log'"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			args.FlagsInit()
			os.Args = tc.osArgs
			var out bytes.Buffer
			require.NoError(t, doit(&out, fSys))

			assert.Contains(t, out.String(), tc.expected)
		})
	}
}
