// SPDX-License-Identifier: MIT

package main

import (
	"fmt"
	"os"
	"path/filepath"

	"github.com/rokath/trice/internal/args"
)

const outputFile = "docs/ref/trice-help-all.txt"

var (
	renderHelpText = args.RenderHelpText
	getWorkingDir  = os.Getwd
	mkdirAll       = os.MkdirAll
	writeFile      = os.WriteFile
)

func run() error {
	text, err := renderHelpText("-all")
	if err != nil {
		return fmt.Errorf("render help text: %w", err)
	}

	root, err := repoRoot()
	if err != nil {
		return fmt.Errorf("locate repository root: %w", err)
	}
	targetFile := filepath.Join(root, outputFile)

	if err := mkdirAll(filepath.Dir(targetFile), 0o755); err != nil {
		return fmt.Errorf("create output directory: %w", err)
	}
	if err := writeFile(targetFile, []byte(text), 0o644); err != nil {
		return fmt.Errorf("write help output: %w", err)
	}
	return nil
}

func main() {
	if err := run(); err != nil {
		fail(err)
	}
}

func repoRoot() (string, error) {
	wd, err := getWorkingDir()
	if err != nil {
		return "", err
	}
	// go generate runs in the package directory of the file containing the directive.
	// The directive lives in internal/args, so the repository root is two levels up.
	return filepath.Clean(filepath.Join(wd, "..", "..")), nil
}

func fail(err error) {
	fmt.Fprintln(os.Stderr, err)
	os.Exit(1)
}
