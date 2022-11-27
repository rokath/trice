// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package trice

// #include <stdint.h>
// #include "trice.h"
// #include "cgoTrice.h"
// #include "messages.h"
// #cgo CFLAGS: -g -Wall -Icgo
import "C"

import (
	"fmt"
	"io"
	"unsafe"
)

// cgoSetTriceBuffer tells the underlying C code where to output the trice byte stream.
func cgoSetTriceBuffer(o []byte) {
	Cout := (*C.uchar)(unsafe.Pointer(&o[0]))
	C.CgoSetTriceBuffer(Cout)
}

// dump prints the byte slice as hex in one line
func dump(w io.Writer, b []byte) {
	fmt.Fprint(w, "exp := []byte{ ")
	for _, x := range b {
		fmt.Fprintf(w, "0x%02x, ", x)
	}
	fmt.Fprintln(w, "}")
}

func init() {
}

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

// Message3 is a Go wrapper.
func Message3(addr uint64, b0 uint8) []byte {
	o := make([]byte, 1024)
	Cout := (*C.uint8_t)(unsafe.Pointer(&o[0]))
	C.CgoSetTriceBuffer(Cout)
	CLen := C.Message3(C.ulonglong(addr), C.uchar(b0))
	len := int(CLen)
	return o[:len]
}
