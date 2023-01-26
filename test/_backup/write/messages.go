// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package write

// #include <stdint.h>
// #include "trice.h"
// #include "cgoTrice.h"
// #include "messages.h"
// #cgo CFLAGS: -g -Wall -I.. -I../../src
import "C"

import (
	"unsafe"
)

// Message0 is a Go wrapper.
func Message0(b0 byte) []byte {
	o := make([]byte, 1024)
	Cout := (*C.uint8_t)(unsafe.Pointer(&o[0]))
	C.CgoSetTriceBuffer(Cout)
	CLen := C.Message0(C.uchar(b0))
	len := int(CLen)
	return o[:len]
}

// Message1 is a Go wrapper.
func Message1(addr uint16, b0 uint8) []byte {
	o := make([]byte, 1024)
	Cout := (*C.uint8_t)(unsafe.Pointer(&o[0]))
	C.CgoSetTriceBuffer(Cout)
	CLen := C.Message1(C.ushort(addr), C.uchar(b0))
	len := int(CLen)
	return o[:len]
}

// Message2 is a Go wrapper.
func Message2(addr uint32, b0 uint8) []byte {
	o := make([]byte, 1024)
	Cout := (*C.uint8_t)(unsafe.Pointer(&o[0]))
	C.CgoSetTriceBuffer(Cout)
	CLen := C.Message2(C.uint(addr), C.uchar(b0))
	len := int(CLen)
	return o[:len]
}
