package main

import (
	"fmt"
	"os"
	"path/filepath"

	"github.com/rokath/trice/internal/args"
)

const outputFile = "docs/ref/trice-help-all.txt"

func main() {
	text, err := args.RenderHelpText("-all")
	if err != nil {
		fail("render help text", err)
	}

	root, err := repoRoot()
	if err != nil {
		fail("locate repository root", err)
	}
	targetFile := filepath.Join(root, outputFile)

	if err := os.MkdirAll(filepath.Dir(targetFile), 0o755); err != nil {
		fail("create output directory", err)
	}
	if err := os.WriteFile(targetFile, []byte(text), 0o644); err != nil {
		fail("write help output", err)
	}
}

func repoRoot() (string, error) {
	wd, err := os.Getwd()
	if err != nil {
		return "", err
	}
	// go generate runs in the package directory of the file containing the directive.
	// The directive lives in internal/args, so the repository root is two levels up.
	return filepath.Clean(filepath.Join(wd, "..", "..")), nil
}

func fail(step string, err error) {
	fmt.Fprintf(os.Stderr, "%s: %v\n", step, err)
	os.Exit(1)
}
