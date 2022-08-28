package tcobs

// #include <stdint.h>
// #include "tcobs.h"
// #cgo CFLAGS: -g -Wall
import "C"
import (
	"unsafe"
)

// todo: -flto switch does not work on some Windows (Antivirus?) setups.

// CEncode encodes `i` into `o` and returns number of bytes in `o`.
// For details see https://github.com/rokath/TCOBS/blob/master/docs/TCOBSSpecification.md.
// The CEncode implementation is done in C because the aimed use case is an embedded device running C.
// This function is mainly for testing.
func CEncode(o, i []byte) (n int) {
	if len(i) == 0 {
		return
	}
	in := unsafe.Pointer(&i[0])
	out := unsafe.Pointer(&o[0])
	n = int(C.TCOBSEncode(out, in, C.size_t(len(i))))
	return
}
