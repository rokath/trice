// Package src is a helper for testing the target C-code.
// Each C function gets a Go wrapper which ist tested in an appropriate test function.
// Calling a C trice function results in putting some bytes into the C fifo.
// The appropriate test function reads the C fifo back and compares the result with the expected values.
// For some reason inside the src_test.go an 'import "C"' is not possible.
package src

// #include "trice.h"
// unsigned SEGGER_RTT_Write(unsigned BufferIndex, const void* pBuffer, unsigned NumBytes){ return 0; } // to make gcc happy
// #cgo CFLAGS: -g -Wall -I config -I intern -I "../../third_party/segger.com/SEGGER_RTT/RTT" -I "../../third_party/cobs-c-0.5.0"
import "C"

// read reads from the C source fifo count bytes
func read(count int) []byte {
	b := make([]byte, count)
	for i := range b {
		b[i] = byte(C.triceU8Pop())
	}
	return b
}

// iD transforms i into the flex id bit pattern what is done normally by the Id(i) macro inside the C soource
func iD(i int) C.uint {
	return C.uint((i << (32 - 21)) | 0x80000000)
}

//  // trice0i calls the C function trice0i with the appropriate transformed id and the format string fmt.
//  // fmt can be an empty string "" because C.trice0i executes the C code behind macro TRICE0i and that ignores fmt.
//  // trice0i returns a byte slice with len byteCount containing the trice transfer bytes.
//  func trice0i(byteCount, id int, fmt string) []byte {
//  	C.trice0i(iD(id), C.CString(fmt))
//  	return read(byteCount)
//  }

// trice0 calls the C function trice0 with the appropriate transformed id and the format string fmt.
// fmt can be an empty string "" because C.trice0 executes the C code behind macro TRICE0i and that ignores fmt.
// trice0 returns a byte slice with len byteCount containing the trice transfer bytes.
func trice0(byteCount, id int, fmt string) []byte {
	C.trice0(iD(id), C.CString(fmt))
	return read(byteCount)
}

//  // trice8_1i calls the C function trice8_1i with the appropriate transformed id and the format string fmt with one byte as parameter.
//  // fmt can be an empty string "" because C.trice8_1i executes the C code behind macro TRICE0i and that ignores fmt.
//  // trice8_1i returns a byte slice with len byteCount containing the trice transfer bytes.
//  func trice8_1i(byteCount, id int, fmt string, b0 byte) []byte {
//  	C.trice8_1i(iD(id), C.CString(fmt), C.schar(b0))
//  	return read(byteCount)
//  }

// trice8_1 calls the C function trice8_1i with the appropriate transformed id and the format string fmt with one byte as parameter.
// fmt can be an empty string "" because C.trice8_1 executes the C code behind macro TRICE0i and that ignores fmt.
// trice8_1 returns a byte slice with len byteCount containing the trice transfer bytes.
func trice8_1(byteCount, id int, fmt string, b0 byte) []byte {
	C.trice8_1(iD(id), C.CString(fmt), C.schar(b0))
	return read(byteCount)
}

//  // trice8_2i calls the C function trice8_2i with the appropriate transformed id and the format string fmt with 2 bytes as parameter.
//  // fmt can be an empty string "" because C.trice8_1i executes the C code behind macro TRICE0i and that ignores fmt.
//  // trice8_2i returns a byte slice with len byteCount containing the trice transfer bytes.
//  func trice8_2i(byteCount, id int, fmt string, b0, b1 byte) []byte {
//  	C.trice8_2i(iD(id), C.CString(fmt), C.schar(b0), C.schar(b1))
//  	return read(byteCount)
//  }

// trice8_2 calls the C function trice8_2 with the appropriate transformed id and the format string fmt with 2 bytes as parameter.
// fmt can be an empty string "" because C.trice8_1 executes the C code behind macro TRICE0i and that ignores fmt.
// trice8_2 returns a byte slice with len byteCount containing the trice transfer bytes.
func trice8_2(byteCount, id int, fmt string, b0, b1 byte) []byte {
	C.trice8_2(iD(id), C.CString(fmt), C.schar(b0), C.schar(b1))
	return read(byteCount)
}
