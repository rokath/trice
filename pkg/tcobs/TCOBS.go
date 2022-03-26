// Package src is a helper for testing the target C-code.
// Each C function gets a Go wrapper which ist tested in an appropriate test function.
// For some reason inside the src_test.go an 'import "C"' is not possible.
package src

// #include <stdint.h>
// #include "TCOBS.h"
// #cgo CFLAGS: -g -Wall -flto
import "C"
import (
	"bytes"
	"unsafe"
)

// TCOBSEncodeC encodes i into o and returns number of bytes in o.
func TCOBSEncodeC(o, i []byte) (n int) {
	if len(i) == 0 {
		return
	}
	in := (*C.uint8_t)(unsafe.Pointer(&i[0]))
	out := (*C.uint8_t)(unsafe.Pointer(&o[0]))
	n = int(C.TCOBSEncode(out, in, C.uint(len(i))))
	return
}

// TCOBSDecode decodes a TCOBS frame i (without 0-delimiter) to o and returns as n the valid data length in o.
func TCOBSDecode(o, i []byte) (n int, e error) {
	return
}

// TCOBSDecoder expects in i a 0-delimited number of TCOBS packages. If a 0-delimiter is found inside i,
// it tries to decode all bytes until the 0-delimiter as TCOBS package and writes the result in o.
// no is the number of valid data in o afterwards and ni is the number of bytes removed from i (including the 0-delimiter).
// If i does not contain any 0-delimiter no and ni are 0 and e is nil.
// If a 0-delimiter was found, but the TCOBS decoding failed, no is 0 and e is not nil.
func TCOBSDecoder(o, i []byte) (no, ni int, e error) {
	x := bytes.SplitN(i, []byte{0}, 1)
	if len(x) == 0 {
		return
	}
	ni = len(x[0]) + 1
	no, e = TCOBSDecode(o, x[0])
	return
}
