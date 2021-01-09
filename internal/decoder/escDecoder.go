// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"encoding/binary"
	"fmt"
	"io"

	"github.com/rokath/trice/internal/id"
)

// Esc is the Decoder instance for esc encoded trices.
type Esc struct {
	decoding
}

// NewEscDecoder provides an EscDecoder instance.
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
func NewEscDecoder(l []id.Item, in io.Reader, endian bool) (r io.Reader) {
	p := &Esc{}
	p.in = in
	p.syncBuffer = make([]byte, 0, defaultSize)
	p.lut = MakeLut(l)
	p.endian = endian // esc format is only big endian
	r = p
	return
}

// Read is the provided read method for esc decoding of next string as byte slice.
// It uses inner reader p.in and internal id look-up table to fill b with a string.
// b is a slice of bytes with a len for the max expected string size.
// n is the count of read bytes inside b.
// Read returns one trice string or nothing.
func (p *Esc) Read(b []byte) (n int, err error) {
	sizeMsg := fmt.Sprintln("e:buf too small, expecting", defaultSize, "bytes.")
	if len(b) < len(sizeMsg) {
		return
	}
	if len(b) < defaultSize {
		n = copy(b, sizeMsg)
		return
	}

	// use b as intermediate read buffer to avoid allocation
	n, err = p.in.Read(b)
	// p.syncBuffer can contain unprocessed bytes from last call.
	p.syncBuffer = append(p.syncBuffer, b[:n]...) // merge with leftovers
	n = 0
	if nil != err && io.EOF != err {
		n = copy(b, fmt.Sprintln("error:internal reader error ", err))
		return
	}

	// Even err could be io.EOF some valid data possibly in p.syncBuffer.
	// In case of file input (JLINK usage) a plug off is not detectable here.

	p.bc = len(p.syncBuffer) // intermediade assingment for better error tracking
	if p.bc < 4 {
		return // wait
	}
	p.b = b
	if 0xec != p.syncBuffer[0] { // 0xec == 236
		return p.outOfSync("start byte is not 0xEC")
	}
	lengthCode := p.syncBuffer[1]
	if 0xde == lengthCode { // 0xde == 222
		return p.outOfSync("0xEC is followed by 0xDE, so no start byte")
	}
	triceID := int(binary.BigEndian.Uint16(p.syncBuffer[2:4]))
	var ok bool
	p.trice, ok = p.lut[triceID]
	if !ok { // unknown id
		return p.outOfSync(fmt.Sprint("unknown ID ", triceID))
	}
	p.bc = p.bytesCount(lengthCode) // payload plus header
	if p.expectedByteCount() != p.bc {
		return p.outOfSync(fmt.Sprint("trice.Type ", p.trice.Type, " with not matching length code ", lengthCode))
	}
	if len(p.syncBuffer) < 4+p.bc { // header plus payload
		return // wait
	}
	// ID and count are ok
	return p.sprintTrice()
}

func (p *Esc) bytesCount(lc byte) int {
	if 0xe0 <= lc && lc <= 0xe9 {
		return 1 << (lc - 0xe0)
	}
	if 0xdf == lc {
		return 0
	}
	return -2
}

// byteCount returns expected byte count for triceType.
// It returns -1 for an unknown value an -2 for unknown triceType.
func (p *Esc) expectedByteCount() int {
	switch p.trice.Type {
	case "TRICE0":
		return 0
	case "TRICE8_1":
		return 1
	case "TRICE8_2", "TRICE16_1":
		return 2
	case "TRICE8_3", "TRICE8_4", "TRICE16_2", "TRICE32_1":
		return 4
	case "TRICE8_5", "TRICE8_6", "TRICE16_3", "TRICE8_7", "TRICE8_8", "TRICE16_4", "TRICE32_2", "TRICE64_1":
		return 8
	case "TRICE32_3", "TRICE32_4", "TRICE64_2":
		return 16
	case "TRICE_S":
		return p.bc
	default:
		return -1 // unknown trice type
	}
}

func (p *Esc) sprintTrice() (n int, e error) {
	if "TRICE_S" == p.trice.Type { // special case
		return p.triceS()
	}
	p.rub(4) // remove header
	switch p.trice.Type {
	case "TRICE0":
		return p.trice0()
	case "TRICE8_1":
		return p.trice81()
	case "TRICE8_2":
		return p.trice82()
	case "TRICE8_3":
		return p.trice83()
	case "TRICE8_4":
		return p.trice84()
	case "TRICE8_5":
		return p.trice85()
	case "TRICE8_6":
		return p.trice86()
	case "TRICE8_7":
		return p.trice87()
	case "TRICE8_8":
		return p.trice88()
	case "TRICE16_1":
		return p.trice161()
	case "TRICE16_2":
		return p.trice162()
	case "TRICE16_3":
		return p.trice163()
	case "TRICE16_4":
		return p.trice164()
	case "TRICE32_1":
		return p.trice321()
	case "TRICE32_2":
		return p.trice322()
	case "TRICE32_3":
		return p.trice323()
	case "TRICE32_4":
		return p.trice324()
	case "TRICE64_1":
		return p.trice641()
	case "TRICE64_2":
		return p.trice642()
	default:
		return p.outOfSync(fmt.Sprintf("Unexpected trice.Type %s", p.trice.Type))
	}
}

func (p *Esc) triceS() (n int, e error) {
	b := p.syncBuffer[4:]

	var i int // find index of first 0 or last index
	for ; i < p.bc && 0 != b[i]; i++ {
	}

	// check first half
	if 4 <= p.bc && i < (p.bc>>1)+1 {
		p.bc += 4
		return p.outOfSync("first half of string buffer and first byte of 2nd half not allowed to contain a 0")
	}

	// check tail
	j := i
	for ; j < p.bc; j++ {
		if 0 != b[j] {
			p.bc += 4
			return p.outOfSync("padding bytes not zero")
		}
	}
	// ok
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, string(b[:i])))
	p.rub(4 + p.bc)
	return
}

func (p *Esc) trice0() (n int, e error) {
	n = copy(p.b, fmt.Sprintf(p.trice.Strg))
	return
}

func (p *Esc) trice81() (n int, e error) {
	b0 := int8(p.syncBuffer[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0))
	p.rub(p.bc)
	return
}

func (p *Esc) trice82() (n int, e error) {
	b0 := int8(p.syncBuffer[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.syncBuffer[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1))
	p.rub(p.bc)
	return
}

func (p *Esc) trice83() (n int, e error) {
	b0 := int8(p.syncBuffer[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.syncBuffer[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.syncBuffer[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.syncBuffer[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	if 0 != b3 {
		return p.outOfSync("padding byte not zero")
	}
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2))
	p.rub(p.bc)
	return
}

func (p *Esc) trice84() (n int, e error) {
	b0 := int8(p.syncBuffer[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.syncBuffer[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.syncBuffer[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.syncBuffer[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3))
	p.rub(p.bc)
	return
}

func (p *Esc) trice85() (n int, e error) {
	b0 := int8(p.syncBuffer[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.syncBuffer[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.syncBuffer[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.syncBuffer[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.syncBuffer[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b5 := int8(p.syncBuffer[5]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b6 := int8(p.syncBuffer[6]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b7 := int8(p.syncBuffer[7]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	if 0 != b7 || 0 != b6 || 0 != b5 {
		return p.outOfSync("padding bytes not zero")
	}
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4))
	p.rub(p.bc)
	return
}

func (p *Esc) trice86() (n int, e error) {
	b0 := int8(p.syncBuffer[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.syncBuffer[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.syncBuffer[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.syncBuffer[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.syncBuffer[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b5 := int8(p.syncBuffer[5]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b6 := int8(p.syncBuffer[6]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b7 := int8(p.syncBuffer[7]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	if 0 != b7 || 0 != b6 {
		return p.outOfSync("padding bytes not zero")
	}
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5))
	p.rub(p.bc)
	return
}

func (p *Esc) trice87() (n int, e error) {
	b0 := int8(p.syncBuffer[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.syncBuffer[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.syncBuffer[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.syncBuffer[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.syncBuffer[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b5 := int8(p.syncBuffer[5]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b6 := int8(p.syncBuffer[6]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b7 := int8(p.syncBuffer[7]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	if 0 != b7 {
		return p.outOfSync("padding byte not zero")
	}
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5, b6))
	p.rub(p.bc)
	return
}

func (p *Esc) trice88() (n int, e error) {
	b0 := int8(p.syncBuffer[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.syncBuffer[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.syncBuffer[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.syncBuffer[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.syncBuffer[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b5 := int8(p.syncBuffer[5]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b6 := int8(p.syncBuffer[6]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b7 := int8(p.syncBuffer[7]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5, b6, b7))
	p.rub(p.bc)
	return
}

func (p *Esc) trice161() (n int, e error) {
	d0 := int16(p.readU16(p.syncBuffer[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0))
	p.rub(p.bc)
	return
}

func (p *Esc) trice162() (n int, e error) {
	d0 := int16(p.readU16(p.syncBuffer[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int16(p.readU16(p.syncBuffer[2:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1))
	p.rub(p.bc)
	return
}

func (p *Esc) trice163() (n int, e error) {
	d0 := int16(p.readU16(p.syncBuffer[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int16(p.readU16(p.syncBuffer[2:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int16(p.readU16(p.syncBuffer[4:6])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d3 := int16(p.readU16(p.syncBuffer[6:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	if 0 != d3 {
		return p.outOfSync("padding bytes not zero")
	}
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2))
	p.rub(p.bc)
	return
}

func (p *Esc) trice164() (n int, e error) {
	d0 := int16(p.readU16(p.syncBuffer[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int16(p.readU16(p.syncBuffer[2:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int16(p.readU16(p.syncBuffer[4:6])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d3 := int16(p.readU16(p.syncBuffer[6:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2, d3))
	p.rub(p.bc)
	return
}

func (p *Esc) trice321() (n int, e error) {
	d0 := int32(p.readU32(p.syncBuffer[0:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0))
	p.rub(p.bc)
	return
}

func (p *Esc) trice322() (n int, e error) {
	d0 := int32(p.readU32(p.syncBuffer[0:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int32(p.readU32(p.syncBuffer[4:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1))
	p.rub(p.bc)
	return
}

func (p *Esc) trice323() (n int, e error) {
	d0 := int32(p.readU32(p.syncBuffer[0:4]))   // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int32(p.readU32(p.syncBuffer[4:8]))   // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int32(p.readU32(p.syncBuffer[8:12]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d3 := int32(p.readU32(p.syncBuffer[12:16])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	if 0 != d3 {
		return p.outOfSync("padding bytes not zero")
	}
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2))
	p.rub(p.bc)
	return
}

func (p *Esc) trice324() (n int, e error) {
	d0 := int32(p.readU32(p.syncBuffer[0:4]))   // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int32(p.readU32(p.syncBuffer[4:8]))   // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int32(p.readU32(p.syncBuffer[8:12]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d3 := int32(p.readU32(p.syncBuffer[12:16])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2, d3))
	p.rub(p.bc)
	return
}

func (p *Esc) trice641() (n int, e error) {
	d0 := int64(p.readU64(p.syncBuffer[0:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0))
	p.rub(p.bc)
	return
}

func (p *Esc) trice642() (n int, e error) {
	d0 := int64(p.readU64(p.syncBuffer[0:8]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int64(p.readU64(p.syncBuffer[8:16])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1))
	p.rub(p.bc)
	return
}
