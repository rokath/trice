package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"os"
	"path/filepath"

	ignore "github.com/sabhiram/go-gitignore"
)

func printHelpAndExit() {
	fmt.Println(`clang-filter – filter file lists using .clang-format-ignore

Usage:
  git ls-files '*.c' '*.h' | clang-filter [options]

Options:
  -ignore-file <path>   Path to ignore file (default: .clang-format-ignore)
  -v                    Verbose output (ignored paths printed to stderr)
  -h, --help            Show this help message and exit

Description:
  clang-filter reads file paths from standard input and filters out any paths
  that match patterns in the specified ignore file, using gitignore semantics.
`)
	os.Exit(0)
}

func main() {
	ignoreFile := flag.String("ignore-file", ".clang-format-ignore", "Path to ignore file (gitignore syntax)")
	verbose := flag.Bool("v", false, "Enable verbose logging to stderr")

	// Manually add --help support (flag package only supports -h)
	flag.Usage = func() {
		printHelpAndExit()
	}

	flag.Parse()

	// If no stdin is provided AND no flags → show help
	if flag.NArg() == 0 && !inputAvailable() && len(os.Args) == 1 {
		printHelpAndExit()
	}

	// If explicitly called with --help → show help
	for _, arg := range os.Args[1:] {
		if arg == "--help" {
			printHelpAndExit()
		}
	}

	//-----------------------------------------------------------------------
	// Load ignore file
	//-----------------------------------------------------------------------
	var ign *ignore.GitIgnore
	if _, err := os.Stat(*ignoreFile); err == nil {
		var errCompile error
		ign, errCompile = ignore.CompileIgnoreFile(*ignoreFile)
		if errCompile != nil {
			log.Fatalf("Failed to compile ignore file %s: %v", *ignoreFile, errCompile)
		}
		if *verbose {
			fmt.Fprintf(os.Stderr, "Using ignore file: %s\n", *ignoreFile)
		}
	} else if os.IsNotExist(err) {
		if *verbose {
			fmt.Fprintf(os.Stderr, "Ignore file %s not found, no paths will be ignored.\n", *ignoreFile)
		}
	} else {
		log.Fatalf("Cannot stat ignore file %s: %v", *ignoreFile, err)
	}

	//-----------------------------------------------------------------------
	// Read from stdin
	//-----------------------------------------------------------------------
	scanner := bufio.NewScanner(os.Stdin)

	for scanner.Scan() {
		line := scanner.Text()
		if line == "" {
			continue
		}

		// Normalize path separators (important for Windows)
		norm := filepath.ToSlash(line)

		if ign != nil && ign.MatchesPath(norm) {
			if *verbose {
				fmt.Fprintf(os.Stderr, "Ignoring %s\n", norm)
			}
			continue
		}

		fmt.Println(line)
	}

	if err := scanner.Err(); err != nil {
		log.Fatalf("Error reading from stdin: %v", err)
	}
}

// Detect whether stdin has piped data.
// If terminal → no input.
func inputAvailable() bool {
	info, err := os.Stdin.Stat()
	if err != nil {
		return false
	}
	return (info.Mode() & os.ModeCharDevice) == 0
}
