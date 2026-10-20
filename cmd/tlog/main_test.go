// SPDX-License-Identifier: MIT

package main

import (
	"bytes"
	"context"
	"os"
	"os/exec"
	"strings"
	"testing"
	"time"

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

// TestMainCLIOutput verifies stderr and exit status through tlog's real main,
// including help requests that must succeed without a dictionary or input.
func TestMainCLIOutput(t *testing.T) {
	for _, test := range []struct {
		name       string
		arguments  []string
		diagnostic string
		helpText   string
	}{
		{name: "unknown option", arguments: []string{"-unknown-option"}, diagnostic: "flag provided but not defined: -unknown-option"},
		{name: "missing value", arguments: []string{"-port"}, diagnostic: "flag needs an argument: -port"},
		{name: "invalid number", arguments: []string{"-baud", "invalid"}, diagnostic: `invalid value "invalid" for flag -baud`},
		{name: "invalid format", arguments: []string{"-logFormat", "invalid"}, diagnostic: `invalid -logFormat "invalid"`},
		{name: "invalid command", arguments: []string{"bind"}, diagnostic: `unexpected argument "bind"`},
		{name: "trailing argument", arguments: []string{"-port", "DEC", "wrong"}, diagnostic: `unexpected argument "wrong"`},
		{name: "unknown version option", arguments: []string{"version", "-unknown-option"}, diagnostic: "flag provided but not defined: -unknown-option"},
		{name: "help command", arguments: []string{"help"}, helpText: "sub-command 'l|log'"},
		{name: "short help", arguments: []string{"-h"}, helpText: "Usage of log:"},
		{name: "long help", arguments: []string{"--help"}, helpText: "Usage of log:"},
	} {
		t.Run(test.name, func(t *testing.T) {
			// A parser regression must fail promptly instead of opening an input.
			ctx, cancel := context.WithTimeout(context.Background(), 10*time.Second)
			defer cancel()
			commandArgs := append([]string{"-test.run=^TestHelperProcessTlogMain$", "--"}, test.arguments...)
			cmd := exec.CommandContext(ctx, os.Args[0], commandArgs...)
			cmd.Env = append(os.Environ(), "GO_WANT_TLOG_HELPER_PROCESS=1")
			cmd.Dir = t.TempDir()
			var stdout, stderr bytes.Buffer
			cmd.Stdout, cmd.Stderr = &stdout, &stderr
			err := cmd.Run()
			require.NoError(t, ctx.Err(), "CLI must stop without starting work")
			if test.helpText != "" {
				require.NoError(t, err, stderr.String())
				assert.Contains(t, stdout.String(), test.helpText)
				assert.Contains(t, stdout.String(), "\n  -")
				assert.NotContains(t, stdout.String(), "for CLI documentation.")
				assert.Empty(t, stderr.String())
				return
			}
			var exitErr *exec.ExitError
			require.ErrorAs(t, err, &exitErr)
			assert.Equal(t, 1, exitErr.ExitCode())
			assert.Empty(t, stdout.String())
			lines := strings.Split(strings.TrimSuffix(stderr.String(), "\n"), "\n")
			require.Len(t, lines, 2, stderr.String())
			assert.Contains(t, lines[0], test.diagnostic)
			assert.Equal(t, "Use tlog help for CLI documentation.", lines[1])
		})
	}
}

// TestHelperProcessTlogMain isolates main's os.Exit behavior from the test runner.
func TestHelperProcessTlogMain(t *testing.T) {
	if os.Getenv("GO_WANT_TLOG_HELPER_PROCESS") != "1" {
		return
	}
	previousArgs := os.Args
	defer func() { os.Args = previousArgs }()
	// The parent passes one test selector, then "--", then the CLI arguments.
	os.Args = append([]string{"tlog"}, os.Args[3:]...)
	main()
}
