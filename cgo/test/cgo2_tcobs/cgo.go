// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

// Package cgo is a helper for testing the target C-code.
// Each C function gets a Go wrapper which is tested in appropriate test functions.
// For some reason inside the trice_test.go an 'import "C"' is not possible.
// All C-files in this folder referring to the trice sources this way avoiding code duplication.
// The Go functions defined here are not exported. They are called by the Go test functions in this package.
// This way the test functions are executing the trice C-code compiled with the triceConfig.h here.
package cgo

// #include <stdint.h>
// void TriceCheck( int n );
// unsigned TriceOutDepth( void );
// void CgoSetTriceBuffer( uint8_t* buf );
// #cgo CFLAGS: -g -Wall -I../../../src
import "C"

import (
	"fmt"
	"io"
	"unsafe"
)

// setTriceBuffer tells the underlying C code where to output the trice byte stream.
func setTriceBuffer(o []byte) {
	Cout := (*C.uchar)(unsafe.Pointer(&o[0]))
	C.CgoSetTriceBuffer(Cout)
}

// triceCheck performs triceCheck C-code sequence n. 
func triceCheck(n int) {
	C.TriceCheck(C.int(n))
}

// triceOutDepth returns the actual out buffer depth.
func triceOutDepth() int {
	return int(C.TriceOutDepth())
}

// dump prints the byte slice as hex in one line
func dump(w io.Writer, b []byte) {
	fmt.Fprint(w, "exp := []byte{ ")
	for _, x := range b {
		fmt.Fprintf(w, "0x%02x, ", x)
	}
	fmt.Fprintln(w, "}")
}
