// SPDX-License-Identifier: MIT

package main

import (
	"bytes"
	"os"
	"os/exec"
	"testing"

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
	default:
		os.Exit(2)
	}
}
