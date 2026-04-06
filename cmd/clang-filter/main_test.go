// SPDX-License-Identifier: MIT

package main

import (
	"bytes"
	"os"
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
