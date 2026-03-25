// SPDX-License-Identifier: MIT

// Package args defines the Trice command-line interface.
//
// It owns subcommand flag sets, parses command-line arguments, and dispatches
// to the corresponding runtime actions (log, help, version, add, insert, clean,
// generate, scan, display server, and shutdown).
//
// Handler is the package entry point used by cmd/trice.
package args
