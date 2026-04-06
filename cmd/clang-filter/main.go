// SPDX-License-Identifier: MIT

package main

import (
	"bufio"
	"errors"
	"flag"
	"fmt"
	"io"
	"log"
	"os"
	"path/filepath"

	ignore "github.com/sabhiram/go-gitignore"
)

//
// clang-filter
//
// Small helper tool to filter file lists using a .clang-format-ignore file
// with gitignore-style syntax.
//
// Typical usage (from the repository root):
//
//   git ls-files '*.c' '*.h' '*.cpp' '*.hpp' | go run ./cmd/clang-filter
//
// -> reads file paths from STDIN
// -> loads .clang-format-ignore from the current working directory
// -> prints only the paths that should NOT be ignored
//

// printHelpAndExit prints a short usage help and exits with code 0.
func printHelp(w io.Writer) {
	fmt.Fprint(w, `clang-filter – filter file lists using .clang-format-ignore

Usage:
  git ls-files '*.c' '*.h' | clang-filter [options]

Options:
  -ignore-file <path>   Path to ignore file (default: .clang-format-ignore)
  -v                    Verbose output (ignored paths printed to stderr)
  -h, --help            Show this help message and exit

Description:
  clang-filter reads file paths from standard input and filters out any paths
  that match patterns in the specified ignore file, using gitignore semantics.

  The typical workflow is:
    1) Use 'git ls-files' to enumerate tracked source/header files.
    2) Pipe them into clang-filter to apply .clang-format-ignore.
    3) Use the resulting list as input for tools like clang-format.
`)
}

// inputAvailable returns true if there is data coming from stdin,
// and false if stdin is connected to a terminal with no pipe.
//
// This is used to decide whether to show the help text by default
// when the tool is invoked without parameters and without piped input.
func inputAvailable() bool {
	return inputAvailableFrom(os.Stdin)
}

func inputAvailableFrom(r *os.File) bool {
	info, err := r.Stat()
	if err != nil {
		return false
	}

	// If stdin is not a character device, there is likely piped data.
	return (info.Mode() & os.ModeCharDevice) == 0
}

var errHelpRequested = errors.New("help requested")

func run(args []string, stdin io.Reader, stdout, stderr io.Writer) error {
	fs := flag.NewFlagSet("clang-filter", flag.ContinueOnError)

	// Define command-line flags with default values.
	ignoreFile := fs.String("ignore-file", ".clang-format-ignore", "Path to ignore file (gitignore syntax)")
	verbose := fs.Bool("v", false, "Enable verbose logging to stderr")

	// Override the default usage function to show our custom help text.
	fs.SetOutput(stderr)
	fs.Usage = func() {
		printHelp(stdout)
	}

	// Parse command-line flags (e.g. -ignore-file, -v, -h).
	if err := fs.Parse(args); err != nil {
		if errors.Is(err, flag.ErrHelp) {
			printHelp(stdout)
			return errHelpRequested
		}
		return err
	}

	// If the user explicitly asks for "--help", we show help and exit.
	for _, arg := range args {
		if arg == "--help" {
			printHelp(stdout)
			return errHelpRequested
		}
	}

	// If there are no arguments AND no piped input, show help by default.
	// This makes the tool user-friendly when someone just runs "./clang-filter"
	// manually without knowing how to use it.
	if !inputAvailable() && len(args) == 0 {
		printHelp(stdout)
		return errHelpRequested
	}

	//---------------------------------------------------------------------------
	// 1) Load / compile the ignore file (if it exists)
	//
	// We use github.com/sabhiram/go-gitignore to implement the gitignore-like
	// matching behavior. If the ignore file does not exist, we simply treat
	// it as "no paths are ignored".
	//---------------------------------------------------------------------------
	var ign *ignore.GitIgnore

	if _, err := os.Stat(*ignoreFile); err == nil {
		compiled, errCompile := ignore.CompileIgnoreFile(*ignoreFile)
		if errCompile != nil {
			log.Fatalf("Failed to compile ignore file %s: %v", *ignoreFile, errCompile)
		}
		ign = compiled

		if *verbose {
			fmt.Fprintf(stderr, "Using ignore file: %s\n", *ignoreFile)
		}
	} else if os.IsNotExist(err) {
		if *verbose {
			fmt.Fprintf(stderr, "Ignore file %s not found, no paths will be ignored.\n", *ignoreFile)
		}
		// ign remains nil: no ignores
	} else {
		return fmt.Errorf("cannot stat ignore file %s: %w", *ignoreFile, err)
	}

	//---------------------------------------------------------------------------
	// 2) Read file paths from stdin, one per line, and filter them
	//---------------------------------------------------------------------------
	scanner := bufio.NewScanner(stdin)

	for scanner.Scan() {
		line := scanner.Text()

		// Skip empty lines (defensive; not strictly required).
		if line == "" {
			continue
		}

		// Normalize path separators to forward slashes for matching.
		// This is important for Windows, where paths use backslashes.
		normPath := filepath.ToSlash(line)

		// If we have a compiled ignore object and it matches this path,
		// we skip printing it (i.e., the file is considered "ignored").
		if ign != nil && ign.MatchesPath(normPath) {
			if *verbose {
				fmt.Fprintf(stderr, "Ignoring %s\n", normPath)
			}
			continue
		}

		// Otherwise, we print the original line (not the normalized one),
		// because the calling script may prefer the exact file name format.
		fmt.Fprintln(stdout, line)
	}

	// Check for scanner errors (e.g. I/O problems on stdin).
	if err := scanner.Err(); err != nil {
		return fmt.Errorf("error reading from stdin: %w", err)
	}
	return nil
}

func main() {
	if err := run(os.Args[1:], os.Stdin, os.Stdout, os.Stderr); err != nil {
		if errors.Is(err, errHelpRequested) {
			os.Exit(0)
		}
		log.Fatal(err)
	}
}
