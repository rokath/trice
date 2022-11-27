// Package trice is a helper for testing the target C-code.
// Each C function gets a Go wrapper which ist tested in appropriate test functions.
// For some reason inside the trice_test.go an 'import "C"' is not possible.
package trice

// #include <stdint.h>
// #include "trice.h"
// #include "trice_test.h"
// #cgo CFLAGS: -g -Wall -Icgo
import "C"

// triceCode performs trice code sequence n. It returns the actual byte stream length.
// It is simply a Go wrapper.
func triceCode(n int) int {
	return int(C.TriceCode(C.int(n)))
}
