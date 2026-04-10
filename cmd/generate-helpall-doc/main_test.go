// SPDX-License-Identifier: MIT

package main

import (
	"bytes"
	"errors"
	"os"
	"os/exec"
	"path/filepath"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
)

func TestRepoRoot(t *testing.T) {
	previous := getWorkingDir
	getWorkingDir = func() (string, error) {
		return filepath.Join("repo", "internal", "args"), nil
	}
	defer func() { getWorkingDir = previous }()

	root, err := repoRoot()
	require.NoError(t, err)
	assert.Equal(t, "repo", root)
}

func TestRunWritesHelpFile(t *testing.T) {
	tmp := t.TempDir()

	prevRender := renderHelpText
	prevGetwd := getWorkingDir
	prevMkdirAll := mkdirAll
	prevWriteFile := writeFile
	defer func() {
		renderHelpText = prevRender
		getWorkingDir = prevGetwd
		mkdirAll = prevMkdirAll
		writeFile = prevWriteFile
	}()

	renderHelpText = func(...string) (string, error) {
		return "help output", nil
	}
	getWorkingDir = func() (string, error) {
		return filepath.Join(tmp, "internal", "args"), nil
	}
	mkdirAll = os.MkdirAll
	writeFile = os.WriteFile

	require.NoError(t, run())

	data, err := os.ReadFile(filepath.Join(tmp, outputFile))
	require.NoError(t, err)
	assert.Equal(t, "help output", string(data))
}

func TestRunPropagatesRenderError(t *testing.T) {
	prevRender := renderHelpText
	defer func() { renderHelpText = prevRender }()
	renderHelpText = func(...string) (string, error) {
		return "", errors.New("boom")
	}

	err := run()
	require.Error(t, err)
	assert.Contains(t, err.Error(), "render help text")
}

func TestRepoRootPropagatesWorkingDirectoryError(t *testing.T) {
	previous := getWorkingDir
	getWorkingDir = func() (string, error) {
		return "", errors.New("boom")
	}
	defer func() { getWorkingDir = previous }()

	_, err := repoRoot()
	require.Error(t, err)
	assert.Contains(t, err.Error(), "boom")
}

func TestRunPropagatesCreateDirectoryError(t *testing.T) {
	prevRender := renderHelpText
	prevGetwd := getWorkingDir
	prevMkdirAll := mkdirAll
	defer func() {
		renderHelpText = prevRender
		getWorkingDir = prevGetwd
		mkdirAll = prevMkdirAll
	}()

	renderHelpText = func(...string) (string, error) {
		return "help output", nil
	}
	getWorkingDir = func() (string, error) {
		return filepath.Join("repo", "internal", "args"), nil
	}
	mkdirAll = func(string, os.FileMode) error {
		return errors.New("mkdir failed")
	}

	err := run()
	require.Error(t, err)
	assert.Contains(t, err.Error(), "create output directory")
}

func TestRunPropagatesWriteFileError(t *testing.T) {
	prevRender := renderHelpText
	prevGetwd := getWorkingDir
	prevMkdirAll := mkdirAll
	prevWriteFile := writeFile
	defer func() {
		renderHelpText = prevRender
		getWorkingDir = prevGetwd
		mkdirAll = prevMkdirAll
		writeFile = prevWriteFile
	}()

	renderHelpText = func(...string) (string, error) {
		return "help output", nil
	}
	getWorkingDir = func() (string, error) {
		return filepath.Join("repo", "internal", "args"), nil
	}
	mkdirAll = func(string, os.FileMode) error { return nil }
	writeFile = func(string, []byte, os.FileMode) error {
		return errors.New("write failed")
	}

	err := run()
	require.Error(t, err)
	assert.Contains(t, err.Error(), "write help output")
}

func TestMainInvokesFailOnRunError(t *testing.T) {
	cmd := exec.Command(os.Args[0], "-test.run=TestHelperProcessGenerateHelpMain")
	cmd.Env = append(os.Environ(),
		"GO_WANT_HELPER_PROCESS=1",
		"TRICE_HELP_HELPER_ACTION=main-fail",
	)

	var stderr bytes.Buffer
	cmd.Stderr = &stderr
	err := cmd.Run()
	require.Error(t, err)

	var exitErr *exec.ExitError
	require.ErrorAs(t, err, &exitErr)
	assert.Equal(t, 1, exitErr.ExitCode())
	assert.Contains(t, stderr.String(), "render help text")
}

func TestFailExitsWithStatusOne(t *testing.T) {
	cmd := exec.Command(os.Args[0], "-test.run=TestHelperProcessGenerateHelpMain")
	cmd.Env = append(os.Environ(),
		"GO_WANT_HELPER_PROCESS=1",
		"TRICE_HELP_HELPER_ACTION=fail",
	)

	var stderr bytes.Buffer
	cmd.Stderr = &stderr
	err := cmd.Run()
	require.Error(t, err)

	var exitErr *exec.ExitError
	require.ErrorAs(t, err, &exitErr)
	assert.Equal(t, 1, exitErr.ExitCode())
	assert.Contains(t, stderr.String(), "helper failure")
}

func TestHelperProcessGenerateHelpMain(t *testing.T) {
	if os.Getenv("GO_WANT_HELPER_PROCESS") != "1" {
		return
	}

	switch os.Getenv("TRICE_HELP_HELPER_ACTION") {
	case "main-fail":
		prevRender := renderHelpText
		renderHelpText = func(...string) (string, error) {
			return "", errors.New("render help text failed")
		}
		defer func() { renderHelpText = prevRender }()
		main()
	case "fail":
		fail(errors.New("helper failure"))
	default:
		os.Exit(2)
	}
}
