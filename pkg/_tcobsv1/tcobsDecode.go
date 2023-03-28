package tcobsv1

import (
	"errors"
	"fmt"
	"os"
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
	orig := in // for error analysis
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
			d[len(d)-n] = repeatByte(offset, in, orig)
			fallthrough
		case R3:
			n++
			d[len(d)-n] = repeatByte(offset, in, orig)
			fallthrough
		case R2:
			r := repeatByte(offset, in, orig)
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
func repeatByte(offset int, in, orig []byte) byte {
	if offset == 0 { // left byte of Ri is a sigil byte (probably N)
		if len(in) >= 2 {
			return in[len(in)-2] // a buffer cannot start with Ri
		}
	} else {
		if len(in) >= 1 {
			return in[len(in)-1]
		}
	}
	fmt.Println("UNEXPECTED decode error with:", orig)
	os.Exit(-1)
	return 0

	//   Nov 14 17:53:23.488185  com26:       stm32g0xx_it.c: 174           rx:USART1 rx: c2 71 7c 07 62 be 70 1c 7a 25 2a 00
	//   Nov 14 17:53:23.517327  com26:               main.c: 166      7741 Executing reveived command ...
	//   inconsistent TCOBSv1 buffer: [156 173 254 236 82 0]
	//   panic: runtime error: index out of range [-2]
	//
	//   goroutine 1 [running]:
	//   github.com/rokath/trice/pkg/tcobsv1.repeatByte(...)
	//           C:/repos/trice/pkg/tcobsv1/tcobsDecode.go:127
	//   github.com/rokath/trice/pkg/tcobsv1.Decode({0xc0003bc000, 0x10000, 0x10000}, {0xc00000e580, 0x9?, 0x10})
	//           C:/repos/trice/pkg/tcobsv1/tcobsDecode.go:95 +0x552
	//   github.com/rokath/trice/internal/trexDecoder.(*trexDec).nextPackage(0xc000194000)
	//           C:/repos/trice/internal/trexDecoder/trexDecoder.go:114 +0x4db
	//   github.com/rokath/trice/internal/trexDecoder.(*trexDec).Read(0xc000194000, {0xc0001ce000, 0x10000, 0x10000})
	//           C:/repos/trice/internal/trexDecoder/trexDecoder.go:168 +0x55
	//   github.com/rokath/trice/internal/translator.decodeAndComposeLoop({0xe068c0, 0xc00009c288}, 0xc0000cc1c0, {0xe07308, 0xc000194000}, 0xc00011dd18?, 0xaadda7?)
	//           C:/repos/trice/internal/translator/translator.go:108 +0xc8
	//   github.com/rokath/trice/internal/translator.Translate({0xe068c0?, 0xc00009c288}, 0xd7e100?, 0xc0000891a0, 0xc0000993b0, 0xc000089320, {0x24c698a4ad8?, 0xc0000864e0})
	//           C:/repos/trice/internal/translator/translator.go:79 +0xa75
	//   github.com/rokath/trice/internal/args.logLoop({0xe068c0?, 0xc00009c288})
	//           C:/repos/trice/internal/args/handler.go:184 +0x7fe
	//   github.com/rokath/trice/internal/args.Handler({0xc000070040, 0x4, 0x4})
	//           C:/repos/trice/internal/args/handler.go:97 +0x254
	//   main.doit({0xe06a20, 0xc00000a018})
	//           C:/repos/trice/cmd/trice/main.go:41 +0x14c
	//   main.main()
	//           C:/repos/trice/cmd/trice/main.go:29 +0x27
	//
	//   ms@DESKTOP-7POEGPB MINGW64 /c/repos/trice (refactor)

}
