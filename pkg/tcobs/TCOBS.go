// Package src is a helper for testing the target C-code.
// Each C function gets a Go wrapper which ist tested in an appropriate test function.
// For some reason inside the src_test.go an 'import "C"' is not possible.
package src

// #include <stdint.h>
// #include "TCOBS.h"
// #cgo CFLAGS: -g -Wall -flto
import "C"
import "unsafe"

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

// // TCOBSDecode a null-terminated frame to a slice of bytes
// func TCOBSDecode(o, i []byte) (n int) {
// 	return
// }
