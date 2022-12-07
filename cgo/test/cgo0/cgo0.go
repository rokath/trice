// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package cgo0 is a helper for testing the target C-code.
// Each C function gets a Go wrapper which ist tested in appropriate test functions.
// For some reason inside the trice_test.go an 'import "C"' is not possible.
package cgo0

// #include <stdint.h>
// #include "trice_test.h"
// #include "cgoTrice.h"
// #cgo CFLAGS: -g -Wall -I.. -I../../../src
import "C"

import (
	"fmt"
	"io"
	"unsafe"
)

// SetTriceBuffer tells the underlying C code where to output the trice byte stream.
func setTriceBuffer(o []byte) {
	Cout := (*C.uchar)(unsafe.Pointer(&o[0]))
	C.CgoSetTriceBuffer(Cout)
}

// TriceCode performs trice code sequence n. It returns the actual byte stream length.
// It is simply a Go wrapper.
func triceCode(n int) int {
	return int(C.TriceCode(C.int(n)))
}

// dump prints the byte slice as hex in one line
func dump(w io.Writer, b []byte) {
	fmt.Fprint(w, "exp := []byte{ ")
	for _, x := range b {
		fmt.Fprintf(w, "0x%02x, ", x)
	}
	fmt.Fprintln(w, "}")
}
