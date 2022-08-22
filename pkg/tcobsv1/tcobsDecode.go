package tcobsv1

import (
	"errors"
)

const (
	N        = 0xA0 // noop sigil byte
	Z1       = 0x20 // 1-zero sigil byte
	Z2       = 0x40 // 2-zero sigil byte
	Z3       = 0x60 // 3-zero sigil byte
	F2       = 0xC0 // 2-FF sigil byte
	F3       = 0xE0 // 3-FF sigil byte
	F4       = 0x80 // 4-FF sigil byte
	R2       = 0x08 // 2-repeat sigil byte
	R3       = 0x10 // 3-repeat sigil byte
	R4       = 0x18 // 4-repeat sigil byte
	Reserved = 0x00 // reserved sigil byte
)

// sigilAndOffset interprets by as sigil byte with offset and returns sigil and offset.
// For details see TCOBSSpecification.md.
func sigilAndOffset(by uint8) (sigil, offset int) {
	b := int(by)
	sigil = b & 0xE0 // 0x11100000
	if sigil == 0 {
		sigil = b & 0xF8 // 0x11111000
		offset = 7 & b   // 0x00000111
		return
	}
	offset = 0x1F & b // 0x00011111
	return
}

// Decode decodes a TCOBS frame `in` (without 0-delimiter) to `d` and returns as `n` the valid data length from the end in `d`.
// ATTENTION: d is filled from the end! decoded := d[len(d)-n:] is the final result.
// In case of an error n is 0. n can be 0 without having an error.
// Framing with 0-delimiter to be done before is assumed, so no 0-checks performed.
// For details see TCOBSSpecification.md.
func Decode(d, in []byte) (n int, e error) {
	var count int
	for {
		if len(in) == 0 {
			return
		}
		next := in[len(in)-1] // get next sigil byte (starting from the end)
		sigil, offset := sigilAndOffset(next)
		if offset+1 > len(in) {
			n = 0
			e = errors.New("sigil chain mismatch")
			return
		}
		in = in[:len(in)-1] // remove sigil byte from buffer
		switch sigil {
		case N:
			goto copyBytes

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
			d[len(d)-n] = repeatByte(offset, in)
			fallthrough
		case R3:
			n++
			d[len(d)-n] = repeatByte(offset, in)
			fallthrough
		case R2:
			r := repeatByte(offset, in)
			n++
			d[len(d)-n] = r
			n++
			d[len(d)-n] = r

			goto copyBytes

		case Reserved:
			n = 0
			e = errors.New("reserved sigil not allowed")
			return
		}

	copyBytes:
		to := len(d) - n - offset
		from := len(in) - offset // sigil byte is already removed
		count += len(in[from:])
		if count > len(d) {
			n = 0
			e = errors.New("data buffer too small")
			return
		}
		n += copy(d[to:], in[from:])
		in = in[:len(in)-offset] // remove copied bytes
		continue
	}
}

// repeatByte returns the value to repeat
func repeatByte(offset int, in []byte) byte {
	if offset == 0 { // left byte of Ri is a sigil byte (probably N)
		return in[len(in)-2] // a buffer cannot start with Ri
	} else {
		return in[len(in)-1]
	}
}
