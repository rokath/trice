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
	"errors"
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

const (
	N        = 0xA0
	Z1       = 0x20
	Z2       = 0x40
	Z3       = 0x60
	F2       = 0xC0
	F3       = 0xE0
	F4       = 0x80
	R2       = 0x08
	R3       = 0x10
	R4       = 0x18
	Reserved = 0x00
)

// sigilAndOffet
func sigilAndOffet(by uint8) (sigil, offset int) {
	b := int(by)
	sigil = b & 0xE0
	if sigil == 0 {
		sigil = b & 0xF8
		offset = 7 & b
		return
	}
	offset = 0x1F & b
	return
}

// TCOBSDecode decodes a TCOBS frame i (without 0-delimiter) to d and returns as n the valid data length in d.
// In case of an error n is 0. n can be 0 without having an error.
func TCOBSDecode(d, in []byte) (n int, e error) {
	for {
		if len(in) == 0 {
			return
		}
		sigil, offset := sigilAndOffet(in[len(in)-1])
		if offset+1 > len(in) {
			e = errors.New("sigil chain mismatch")
			return
		}
		in = in[:len(in)-1] // remove sigil byte
		switch sigil {
		case N:
copyBytes:
			for i := 1; i < offset+1; i++ {
				n++
				d[len(d)-n] = in[len(in)-i]
			}
			in = in[:len(in)-offset]
			continue

		case Z3:
			n++
			d[len(d)-n] = 0
			fallthrough
		case Z2:
			n++
			d[len(d)-n] = 0
			fallthrough
		case Z1:
			n++
			d[len(d)-n] = 0
			goto copyBytes

		case F4:
			n++
			d[len(d)-n] = 0xFF
			fallthrough
		case F3:
			n++
			d[len(d)-n] = 0xFF
			fallthrough
		case F2:
			n++
			d[len(d)-n] = 0xFF
			n++
			d[len(d)-n] = 0xFF
			goto copyBytes

		case R4:
			n++
			d[len(d)-n] = in[len(in)-1]
		case R3:
			n++
			d[len(d)-n] = in[len(in)-1]
		case R2:
			n++
			d[len(d)-n] = in[len(in)-1]
			n++
			d[len(d)-n] = in[len(in)-1]
			in = in[:len(in)-1]
			goto copyBytes

		case Reserved:
			e = errors.New("reserved sigil not allowed")
			return
		}
	}
}

// TCOBSDecoder expects in s a 0-delimited number of TCOBS packages. If a 0-delimiter is found inside s,
// it tries to decode all bytes until the 0-delimiter as TCOBS package and writes the result in decoded.
// no is the number of valid data in o afterwards and ni is the number of bytes removed from i (including the 0-delimiter).
// If s does not contain any 0-delimiter decoded is nil, after is s and e is nil.
// If a 0-delimiter was found, but the TCOBS decoding failed, no is 0 and e is not nil.
func TCOBSDecoder(s []byte) (decoded, after []byte, e error) {
	tcobs, after, found := bytes.Cut(s, []byte{0})
	if !found {
		after = s
		return
	}
	decoded = make([]byte, 2*len(tcobs))
	n, e := TCOBSDecode(decoded, tcobs)
	decoded = decoded[:n]
	return
}
