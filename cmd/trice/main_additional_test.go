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

func TestDoitInjectsBuildMetadata(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer setupTest(t, fSys)()

	previousVersion := version
	previousCommit := commit
	previousDate := date
	previousBranch := branch
	previousGitState := gitState
	previousGitStatus := gitStatus
	previousBuiltBy := builtBy
	t.Cleanup(func() {
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
	branch = "origin/main"
	gitState = "dirty"
	gitStatus = "M file.go"
	builtBy = "unit-test"

	previousArgs := os.Args
	os.Args = []string{"trice", "version", "-v"}
	defer func() { os.Args = previousArgs }()

	var out bytes.Buffer
	require.NoError(t, doit(&out, fSys))

	assert.Contains(t, out.String(), "commit=abc1234")
	assert.Contains(t, out.String(), "built at 2026-04-05T23:00:00+02:00")
	assert.Contains(t, out.String(), "(built by unit-test)")
	assert.Contains(t, out.String(), "modified files at build time:")
	assert.Equal(t, version, args.Version)
	assert.Equal(t, commit, args.Commit)
	assert.Equal(t, date, args.Date)
	assert.Equal(t, branch, args.Branch)
	assert.Equal(t, gitState, args.GitState)
	assert.Equal(t, gitStatus, args.GitStatus)
	assert.Equal(t, builtBy, args.BuiltBy)
}

func TestDoitReturnsHandlerError(t *testing.T) {
	fSys := &afero.Afero{Fs: afero.NewMemMapFs()}
	defer setupTest(t, fSys)()

	previousArgs := os.Args
	os.Args = []string{"trice", "definitely-not-a-command"}
	defer func() { os.Args = previousArgs }()

	err := doit(&bytes.Buffer{}, fSys)
	require.Error(t, err)
	assert.Contains(t, err.Error(), "unknown sub-command")
}

func TestMainSuccess(t *testing.T) {
	cmd := exec.Command(os.Args[0], "-test.run=TestHelperProcessTriceMain")
	cmd.Env = append(os.Environ(),
		"GO_WANT_HELPER_PROCESS=1",
		"TRICE_MAIN_HELPER_ACTION=success",
	)

	var stdout bytes.Buffer
	cmd.Stdout = &stdout
	err := cmd.Run()
	require.NoError(t, err)
	assert.NotEmpty(t, stdout.String())
}

func TestMainFailure(t *testing.T) {
	cmd := exec.Command(os.Args[0], "-test.run=TestHelperProcessTriceMain")
	cmd.Env = append(os.Environ(),
		"GO_WANT_HELPER_PROCESS=1",
		"TRICE_MAIN_HELPER_ACTION=failure",
	)

	var stderr bytes.Buffer
	cmd.Stderr = &stderr
	err := cmd.Run()
	require.Error(t, err)

	var exitErr *exec.ExitError
	require.ErrorAs(t, err, &exitErr)
	assert.Equal(t, 1, exitErr.ExitCode())
	assert.Contains(t, stderr.String(), "unknown sub-command")
}

// TestMainCLIOutput checks the real entry point in a separate process so an
// accidental flag-package exit or extra usage dump cannot pass handler tests.
func TestMainCLIOutput(t *testing.T) {
	for _, test := range []struct {
		name        string
		arguments   []string
		diagnostic  string
		helpCommand string
		helpText    string
	}{
		{name: "missing command", diagnostic: "no sub-command provided", helpCommand: "trice help"},
		{name: "unknown command", arguments: []string{"wrong"}, diagnostic: "unknown sub-command 'wrong'", helpCommand: "trice help"},
		{name: "unknown bind option", arguments: []string{"bind", "-unknown-option"}, diagnostic: "flag provided but not defined: -unknown-option", helpCommand: "trice help -bind"},
		{name: "missing bind value", arguments: []string{"b", "-src"}, diagnostic: "flag needs an argument: -src", helpCommand: "trice help -bind"},
		{name: "invalid ID minimum", arguments: []string{"bind", "-IDMin", "invalid"}, diagnostic: `invalid value "invalid" for flag -IDMin`, helpCommand: "trice help -bind"},
		{name: "overflowing ID maximum", arguments: []string{"insert", "-IDMax", "999999999999999999999"}, diagnostic: `invalid value "999999999999999999999" for flag -IDMax`, helpCommand: "trice help -insert"},
		{name: "unexpected argument", arguments: []string{"scan", "wrong"}, diagnostic: `unexpected argument "wrong"`, helpCommand: "trice help -scan"},
		{name: "unknown log option", arguments: []string{"l", "-unknown-option"}, diagnostic: "flag provided but not defined: -unknown-option", helpCommand: "trice help -log"},
		{name: "invalid number", arguments: []string{"log", "-baud", "invalid"}, diagnostic: `invalid value "invalid" for flag -baud`, helpCommand: "trice help -log"},
		{name: "unknown help option", arguments: []string{"help", "-unknown-option"}, diagnostic: "flag provided but not defined: -unknown-option", helpCommand: "trice help -help"},
		{name: "unknown version option", arguments: []string{"--version", "-unknown-option"}, diagnostic: "flag provided but not defined: -unknown-option", helpCommand: "trice help -version"},
		{name: "general help", arguments: []string{"help"}, helpText: "syntax: 'trice sub-command'"},
		{name: "root help", arguments: []string{"--help"}, helpText: "sub-command 'h|help'"},
		{name: "topic help", arguments: []string{"help", "-bind"}, helpText: "sub-command 'b|bind'"},
		{name: "flag help", arguments: []string{"bind", "-h"}, helpText: "Usage of bind:"},
		{name: "scan help", arguments: []string{"scan", "--help"}, helpText: "Usage of scan:"},
	} {
		t.Run(test.name, func(t *testing.T) {
			// Bound a regression that starts a server or logger instead of exiting.
			ctx, cancel := context.WithTimeout(context.Background(), 10*time.Second)
			defer cancel()
			commandArgs := append([]string{"-test.run=^TestHelperProcessTriceMain$", "--"}, test.arguments...)
			cmd := exec.CommandContext(ctx, os.Args[0], commandArgs...)
			cmd.Env = append(os.Environ(), "GO_WANT_HELPER_PROCESS=1", "TRICE_MAIN_HELPER_ACTION=cli")
			cmd.Dir = t.TempDir()
			var stdout, stderr bytes.Buffer
			cmd.Stdout, cmd.Stderr = &stdout, &stderr
			err := cmd.Run()
			require.NoError(t, ctx.Err(), "CLI must stop without starting work")
			if test.helpText != "" {
				require.NoError(t, err, stderr.String())
				assert.Contains(t, stdout.String(), test.helpText)
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
			assert.Equal(t, "Use "+test.helpCommand+" for CLI documentation.", lines[1])
		})
	}
}

func TestHelperProcessTriceMain(t *testing.T) {
	if os.Getenv("GO_WANT_HELPER_PROCESS") != "1" {
		return
	}

	previousArgs := os.Args
	previousVersion := version
	previousCommit := commit
	previousDate := date
	previousBranch := branch
	previousGitState := gitState
	previousGitStatus := gitStatus
	previousBuiltBy := builtBy
	defer func() {
		os.Args = previousArgs
		version = previousVersion
		commit = previousCommit
		date = previousDate
		branch = previousBranch
		gitState = previousGitState
		gitStatus = previousGitStatus
		builtBy = previousBuiltBy
	}()

	switch os.Getenv("TRICE_MAIN_HELPER_ACTION") {
	case "success":
		version = "v9.9.9"
		os.Args = []string{"trice", "version"}
		main()
	case "failure":
		os.Args = []string{"trice", "definitely-not-a-command"}
		main()
	case "cli":
		// The parent passes one test selector, then "--", then the CLI arguments.
		os.Args = append([]string{"trice"}, os.Args[3:]...)
		main()
	default:
		os.Exit(2)
	}
}
