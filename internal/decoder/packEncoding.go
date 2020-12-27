// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"encoding/binary"
	"fmt"
	"io"

	"github.com/rokath/trice/internal/id"
)

const (
	syncPacket = "inf:[SYNCPACKET 0x89abcdef]"
)

// Pack is the Decoder instance for bare encoded trices.
type Pack struct {
	decoding
}

// NewPackEncoding provides an decoder instance.
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
func NewPackEncoding(l []id.Item, in io.Reader) (p *Pack) {
	p = &Pack{}
	p.in = in
	p.syncBuffer = make([]byte, 0, 2*buffSize)
	p.lut = MakeLut(l)
	return
}

// StringsRead is the provided read method for pack decoding.
// It uses method Read to fill ss.
// ss is a slice of strings with a len for the max expected strings.
// Each ss substring can start with a channel specifier.
// m is the count of decoded strings inside ss.
func (p *Pack) StringsRead(ss []string) (m int, err error) {
	for m < len(ss) {
		b := make([]byte, 1000)
		var n int
		n, err = p.Read(b)
		if 0 == n {
			return
		}
		if syncPacket == string(b) {
			continue
		}
		ss[m] = string(b[:n])
		m++
		if nil != err {
			return
		}
	}
	return
}

// byteCount returns expected byte count for triceType.
// It returns -1 for an unknown value an -2 for unknown triceType.
func expectedByteCount(triceType string) int {
	switch triceType {
	case "TRICE0":
		return 0
	case "TRICE8_1":
		return 1
	case "TRICE8_2", "TRICE16_1":
		return 2
	case "TRICE8_3":
		return 3
	case "TRICE8_4", "TRICE16_2", "TRICE32_1":
		return 4
	case "TRICE8_5":
		return 5
	case "TRICE8_6", "TRICE16_3":
		return 6
	case "TRICE8_7":
		return 7
	case "TRICE8_8", "TRICE16_4", "TRICE32_2", "TRICE64_1":
		return 8
	case "TRICE32_3":
		return 12
	case "TRICE32_4", "TRICE64_2":
		return 16
	case "TRICE_S":
		return -1 // unknown count
	default:
		return -2 // unknown trice type
	}
}

// rub removes leading bytes from sync buffer
func (p *Pack) rub(n int) {
	p.syncBuffer = p.syncBuffer[:n]
}

func (p *Pack) paddingBytesOk(triceType string) bool {
	switch triceType {
	case "TRICE8_1", "TRICE8_5":
		return 0 == p.syncBuffer[5] && 0 == p.syncBuffer[6] && 0 == p.syncBuffer[7]
	case "TRICE8_2", "TRICE8_6", "TRICE16_1":
		return 0 == p.syncBuffer[6] && 0 == p.syncBuffer[7]
	case "TRICE8_3", "TRICE8_7":
		return 0 == p.syncBuffer[7]
	case "TRICE16_3":
		return 0 == p.syncBuffer[10] && 0 == p.syncBuffer[11]
	}
	return false
}

// Read is the provided read method for pack decoding of next string as byte slice.
// It uses inner reader and internal id look-up table to fill b with a string.
// b is a slice of bytes with a len for the max expected string size.
// n is the count of read bytes inside b.
func (p *Pack) Read(b []byte) (n int, err error) {
	if 0 == len(b) {
		return
	}

	// fill intermediate read buffer for pack encoding
	rb := make([]byte, 1000)
	var m int
	m, err = p.in.Read(rb)

	// p.syncBuffer contains unprocessed bytes from last call.
	p.syncBuffer = append(p.syncBuffer, rb[:m]...) // merge with leftovers
	if nil != err {
		return
	}
	if len(p.syncBuffer) < 4 {
		return
	}

	triceID := int(binary.BigEndian.Uint16(p.syncBuffer[0:1])) // first 2 bytes are the ID
	count := int(binary.BigEndian.Uint16(p.syncBuffer[2:3]))   // next 2 bytes are the count

	if 0x89ab == triceID && 0xcdef == count {
		p.rub(4)
		n = copy(b, syncPacket)
		return
	}

	trice, ok := p.lut[triceID] // check lookup table
	if !ok {
		n = copy(b, fmt.Sprintln("error:unknown triceID ", triceID, ", ignoring byte", p.syncBuffer[0]))
		p.rub(1)
		return
	}

	byteCount := expectedByteCount(trice.Type)
	if count != byteCount && -1 != byteCount {
		n = copy(b, fmt.Sprint("error:unecpected byteCount ", byteCount, ", ignoring byte ", p.syncBuffer[0]))
		p.rub(1)
		return
	}

	if !p.paddingBytesOk(trice.Type) {
		n = copy(b, fmt.Sprint("error:padding bytes not zero, ignoring byte", p.syncBuffer[0]))
		p.rub(1)
		return
	}

	// ID and count are ok
	switch trice.Type {
	case "TRICE0":
		n = copy(b, fmt.Sprintf(trice.Strg))
		p.rub(4)
		return
	case "TRICE8_1":
		n = copy(b, fmt.Sprintf(trice.Strg, p.syncBuffer[4]))
		p.rub(8)
		return
	case "TRICE8_2":
		n = copy(b, fmt.Sprintf(trice.Strg, p.syncBuffer[4], p.syncBuffer[5]))
		p.rub(8)
		return
	case "TRICE16_1":
		n = copy(b, fmt.Sprintf(trice.Strg, int16(binary.BigEndian.Uint16(p.syncBuffer[4:6]))))
		p.rub(8)
		return
	case "TRICE8_3":
		n = copy(b, fmt.Sprintf(trice.Strg, p.syncBuffer[4], p.syncBuffer[5], p.syncBuffer[6]))
		p.rub(8)
		return
	case "TRICE8_4":
		n = copy(b, fmt.Sprintf(trice.Strg,
			p.syncBuffer[4], p.syncBuffer[5], p.syncBuffer[6], p.syncBuffer[7]))
		p.rub(8)
		return
	case "TRICE16_2":
		n = copy(b, fmt.Sprintf(trice.Strg,
			int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])),
			int16(binary.BigEndian.Uint16(p.syncBuffer[6:8]))))
		p.rub(8)
		return
	case "TRICE32_1":
		vH := int32(binary.BigEndian.Uint16(p.syncBuffer[4:6])) << 16
		vL := int32(binary.BigEndian.Uint16(p.syncBuffer[6:8]))
		n = copy(b, fmt.Sprintf(trice.Strg, vH|vL))
		p.rub(8)
		return
	case "TRICE8_5":
		n = copy(b, fmt.Sprintf(trice.Strg,
			p.syncBuffer[4], p.syncBuffer[5], p.syncBuffer[6], p.syncBuffer[7],
			p.syncBuffer[8]))
		p.rub(12)
		return
	case "TRICE8_6":
		n = copy(b, fmt.Sprintf(trice.Strg,
			p.syncBuffer[4], p.syncBuffer[5], p.syncBuffer[6], p.syncBuffer[7],
			p.syncBuffer[8], p.syncBuffer[9]))
		p.rub(12)
		return
	case "TRICE16_3":
		n = copy(b, fmt.Sprintf(trice.Strg,
			int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])),
			int16(binary.BigEndian.Uint16(p.syncBuffer[6:8])),
			int16(binary.BigEndian.Uint16(p.syncBuffer[8:10]))))
		p.rub(12)
		return
	case "TRICE8_7":
		n = copy(b, fmt.Sprintf(trice.Strg,
			p.syncBuffer[4], p.syncBuffer[5], p.syncBuffer[6], p.syncBuffer[7],
			p.syncBuffer[8], p.syncBuffer[9], p.syncBuffer[10]))
		p.rub(12)
		return
	case "TRICE8_8":
		n = copy(b, fmt.Sprintf(trice.Strg,
			p.syncBuffer[4], p.syncBuffer[5], p.syncBuffer[6], p.syncBuffer[7],
			p.syncBuffer[8], p.syncBuffer[9], p.syncBuffer[10], p.syncBuffer[11]))
		p.rub(12)
		return
	case "TRICE16_4":
		n = copy(b, fmt.Sprintf(trice.Strg,
			int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])),
			int16(binary.BigEndian.Uint16(p.syncBuffer[6:8])),
			int16(binary.BigEndian.Uint16(p.syncBuffer[8:10])),
			int16(binary.BigEndian.Uint16(p.syncBuffer[10:12]))))
		p.rub(12)
		return
	case "TRICE32_2":
		v0H := int32(binary.BigEndian.Uint16(p.syncBuffer[4:6])) << 16
		v0L := int32(binary.BigEndian.Uint16(p.syncBuffer[6:8]))
		v1H := int32(binary.BigEndian.Uint16(p.syncBuffer[8:10])) << 16
		v1L := int32(binary.BigEndian.Uint16(p.syncBuffer[10:12]))
		n = copy(b, fmt.Sprintf(trice.Strg, v0H|v0L, v1H|v1L))
		p.rub(12)
		return
	case "TRICE64_1":
		vHH := int64(binary.BigEndian.Uint16(p.syncBuffer[4:6])) << 48
		vHL := int64(binary.BigEndian.Uint16(p.syncBuffer[6:8])) << 32
		vLH := int64(binary.BigEndian.Uint16(p.syncBuffer[8:10])) << 16
		vLL := int64(binary.BigEndian.Uint16(p.syncBuffer[10:12]))
		n = copy(b, fmt.Sprintf(trice.Strg, vHH|vHL|vLH|vLL))
		p.rub(12)
		return
	case "TRICE32_3":
		v0H := int32(binary.BigEndian.Uint16(p.syncBuffer[4:6])) << 16
		v0L := int32(binary.BigEndian.Uint16(p.syncBuffer[6:8]))
		v1H := int32(binary.BigEndian.Uint16(p.syncBuffer[8:10])) << 16
		v1L := int32(binary.BigEndian.Uint16(p.syncBuffer[10:12]))
		v2H := int32(binary.BigEndian.Uint16(p.syncBuffer[12:14])) << 16
		v2L := int32(binary.BigEndian.Uint16(p.syncBuffer[14:16]))
		n = copy(b, fmt.Sprintf(trice.Strg, v0H|v0L, v1H|v1L, v2H|v2L))
		p.rub(16)
		return
	case "TRICE32_4":
		v0H := int32(binary.BigEndian.Uint16(p.syncBuffer[4:6])) << 16
		v0L := int32(binary.BigEndian.Uint16(p.syncBuffer[6:8]))
		v1H := int32(binary.BigEndian.Uint16(p.syncBuffer[8:10])) << 16
		v1L := int32(binary.BigEndian.Uint16(p.syncBuffer[10:12]))
		v2H := int32(binary.BigEndian.Uint16(p.syncBuffer[12:14])) << 16
		v2L := int32(binary.BigEndian.Uint16(p.syncBuffer[14:16]))
		v3H := int32(binary.BigEndian.Uint16(p.syncBuffer[16:18])) << 16
		v3L := int32(binary.BigEndian.Uint16(p.syncBuffer[18:20]))
		n = copy(b, fmt.Sprintf(trice.Strg, v0H|v0L, v1H|v1L, v2H|v2L, v3H|v3L))
		p.rub(20)
		return
	case "TRICE64_2":
		v0HH := int64(binary.BigEndian.Uint16(p.syncBuffer[4:6])) << 48
		v0HL := int64(binary.BigEndian.Uint16(p.syncBuffer[6:8])) << 32
		v0LH := int64(binary.BigEndian.Uint16(p.syncBuffer[8:10])) << 16
		v0LL := int64(binary.BigEndian.Uint16(p.syncBuffer[10:12]))
		v1HH := int64(binary.BigEndian.Uint16(p.syncBuffer[12:14])) << 48
		v1HL := int64(binary.BigEndian.Uint16(p.syncBuffer[14:16])) << 32
		v1LH := int64(binary.BigEndian.Uint16(p.syncBuffer[16:18])) << 16
		v1LL := int64(binary.BigEndian.Uint16(p.syncBuffer[18:20]))
		n = copy(b, fmt.Sprintf(trice.Strg, v0HH|v0HL|v0LH|v0LL, v1HH|v1HL|v1LH|v1LL))
		p.rub(20)
		return
	case "TRICE_S":
		n = copy(b, fmt.Sprintf(trice.Strg, string(p.syncBuffer[4:4+count])))
		count += 3
		count &= 4
		p.rub(4 + count)
		return
	}
	n = copy(b, fmt.Sprint("error:Unexpected trice.Type ", trice.Type, ", ignoring byte", p.syncBuffer[0]))
	p.rub(1)
	return
}
