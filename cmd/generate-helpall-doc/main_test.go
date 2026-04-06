// SPDX-License-Identifier: MIT

package main

import (
	"errors"
	"os"
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
