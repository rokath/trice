// Package src is a helper for testing the target C-code.
package src

// #include "trice.h"
// unsigned SEGGER_RTT_Write(unsigned BufferIndex, const void* pBuffer, unsigned NumBytes){ return 0; } // to make gcc happy
// #cgo CFLAGS: -g -Wall -I intern -I "C:/repos/trice/third_party/segger.com/SEGGER_RTT/RTT"
import "C"

func read(count int) []byte {
	b := make([]byte, count)
	for i := range b {
		b[i] = byte(C.triceU8Pop())
	}
	return b
}

func iD(i int) C.uint {
	return C.uint((i << (32 - 21)) | 0x80000000)
}

func trice0i(byteCount, id int, fmt string) []byte {
	C.trice0i(iD(id), C.CString(fmt))
	return read(byteCount)
}
