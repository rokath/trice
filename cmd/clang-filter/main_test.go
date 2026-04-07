// SPDX-License-Identifier: MIT

package main

import (
	"bytes"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestPrintHelp(t *testing.T) {
	var out bytes.Buffer
	printHelp(&out)
	assert.Contains(t, out.String(), "clang-filter")
	assert.Contains(t, out.String(), ".clang-format-ignore")
}

func TestInputAvailableFromPipeAndRegularFile(t *testing.T) {
	r, w, err := os.Pipe()
	require.NoError(t, err)
	defer r.Close()
	defer w.Close()
	assert.True(t, inputAvailableFrom(r))

	file := filepath.Join(t.TempDir(), "input.txt")
	require.NoError(t, os.WriteFile(file, []byte("data"), 0o644))
	f, err := os.Open(file)
	require.NoError(t, err)
	defer f.Close()
	assert.True(t, inputAvailableFrom(f))
}

func TestRunFiltersIgnoredPaths(t *testing.T) {
	wd, err := os.Getwd()
	require.NoError(t, err)
	tmp := t.TempDir()
	require.NoError(t, os.Chdir(tmp))
	defer func() { _ = os.Chdir(wd) }()

	require.NoError(t, os.WriteFile(".clang-format-ignore", []byte("ignored.c\nsubdir/\n"), 0o644))

	input := "keep.c\nignored.c\nsubdir/file.c\n"
	var stdout, stderr bytes.Buffer
	err = run([]string{"-v"}, strings.NewReader(input), &stdout, &stderr)
	require.NoError(t, err)

	assert.Equal(t, "keep.c\n", stdout.String())
	assert.Contains(t, stderr.String(), "Using ignore file: .clang-format-ignore")
	assert.Contains(t, stderr.String(), "Ignoring ignored.c")
	assert.Contains(t, stderr.String(), "Ignoring subdir/file.c")
}

func TestRunWithoutIgnoreFileStillPrintsInput(t *testing.T) {
	wd, err := os.Getwd()
	require.NoError(t, err)
	tmp := t.TempDir()
	require.NoError(t, os.Chdir(tmp))
	defer func() { _ = os.Chdir(wd) }()

	var stdout, stderr bytes.Buffer
	err = run([]string{"-ignore-file", "missing.ignore", "-v"}, strings.NewReader("keep.c\n"), &stdout, &stderr)
	require.NoError(t, err)

	assert.Equal(t, "keep.c\n", stdout.String())
	assert.Contains(t, stderr.String(), "Ignore file missing.ignore not found")
}

func TestRunHelpRequested(t *testing.T) {
	var stdout, stderr bytes.Buffer
	err := run([]string{"--help"}, strings.NewReader(""), &stdout, &stderr)
	require.ErrorIs(t, err, errHelpRequested)
	assert.Contains(t, stdout.String(), "Usage:")
}

func TestMainHelpExitCodeZero(t *testing.T) {
	cmd := exec.Command(os.Args[0], "-test.run=TestHelperProcessClangFilterMain")
	cmd.Env = append(os.Environ(),
		"GO_WANT_HELPER_PROCESS=1",
		"TRICE_CLANG_FILTER_HELPER_ACTION=help",
	)

	var stdout bytes.Buffer
	cmd.Stdout = &stdout
	err := cmd.Run()
	require.NoError(t, err)
	assert.Contains(t, stdout.String(), "Usage:")
}

func TestMainFailureExitCodeOne(t *testing.T) {
	cmd := exec.Command(os.Args[0], "-test.run=TestHelperProcessClangFilterMain")
	cmd.Env = append(os.Environ(),
		"GO_WANT_HELPER_PROCESS=1",
		"TRICE_CLANG_FILTER_HELPER_ACTION=failure",
	)

	var stderr bytes.Buffer
	cmd.Stderr = &stderr
	err := cmd.Run()
	require.Error(t, err)

	var exitErr *exec.ExitError
	require.ErrorAs(t, err, &exitErr)
	assert.Equal(t, 1, exitErr.ExitCode())
	assert.Contains(t, stderr.String(), "cannot stat ignore file")
}

func TestHelperProcessClangFilterMain(t *testing.T) {
	if os.Getenv("GO_WANT_HELPER_PROCESS") != "1" {
		return
	}

	previousArgs := os.Args
	defer func() { os.Args = previousArgs }()

	switch os.Getenv("TRICE_CLANG_FILTER_HELPER_ACTION") {
	case "help":
		os.Args = []string{"clang-filter", "--help"}
		main()
	case "failure":
		os.Args = []string{"clang-filter", "-ignore-file", string([]byte{0})}
		main()
	default:
		os.Exit(2)
	}
}
