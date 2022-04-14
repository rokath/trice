// Package src is a helper for testing the target C-code.
// Each C function gets a Go wrapper which ist tested in appropriate test functions.
// For some reason inside the trice_test.go an 'import "C"' is not possible.
package src

// #include <stdint.h>
// #include "trice.h"
// #cgo CFLAGS: -g -Wall -flto
import "C"
import (
	"unsafe"
)

/*
var theCArray *C.YourType = C.getTheArray()
        length := C.getTheArrayLength()
        slice := (*[1 << 30]C.YourType)(unsafe.Pointer(theCArray))[:length:length]

*/

func TriceTestFunction0(o []byte) int {
	out := (*C.uint8_t)(unsafe.Pointer(&o[0]))
	return int(C.TriceTestFunction0(out))
}

/* Encode encodes `i` into `o` and returns number of bytes in `o`.
// For details see TCOBSSpecification.md.
// The Encode implementation is done in C because the aimed use case is an embedded device running C.
// This function is mainly for testing.
func Encode(o, i []byte) (n int) {
	if len(i) == 0 {
		return
	}
	in := (*C.uint8_t)(unsafe.Pointer(&i[0]))
	out := (*C.uint8_t)(unsafe.Pointer(&o[0]))
	n = int(C.TCOBSEncode(out, in, C.uint(len(i))))
	return
}
*/
