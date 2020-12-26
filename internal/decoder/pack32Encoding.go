// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"encoding/binary"
	"fmt"
	"io"

	"github.com/rokath/trice/internal/id"
)

// Bare is the Decoder instance for bare encoded trices.
type Pack32 struct {
	decoding
}

// NewBareFormat provides an BareDecoder instance.
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
func NewPack32Encoding(l []id.Item, in io.Reader) (p *Pack32) {
	p = &Pack32{}
	p.in = in
	p.syncBuffer = make([]byte, 0, 2*buffSize)
	p.lut = MakeLut(l)
	return
}

// StringsRead is the provided read method for bare decoding.
// It uses inner reader and internal id look-up table to fill ss.
// ss is a slice of strings with a len for the max expected strings.
// m is the count of decoded strings inside ss.
func (p *Pack32) StringsRead(ss []string) (m int, err error) {
	// p.syncBuffer contains unprocessed bytes.
	// This could be the max size -1 for one trice (TRICE64_2 is 32 bytes long), so 31 bytes
	maxBytes := 4*len(ss) - len(p.syncBuffer) // one bare trice is 4 bytes: IDHi IDLo b0 b1, so surely read not more than space in ss
	if !(10 < maxBytes && maxBytes < 1000) {
		maxBytes = 1000
	}
	rb := make([]byte, maxBytes)
	var n int
	n, err = p.in.Read(rb)
	p.syncBuffer = append(p.syncBuffer, rb[:n]...) // merge with leftovers
	if nil != err {
		return
	}

	for { // scan p.syncBuffer
		var triceID int // read trice triceID
		var index int
		var trice idFmt
		var ok bool
		var count int
		for len(p.syncBuffer) >= index+4 {
			if len(ss) == m { // no space in string slice
				return
			}
			// the 2 triceID bytes are in bigendian format (network byte order)
			//triceID = (int(p.syncBuffer[index+0]) << 8) | int(p.syncBuffer[index+1]) // to do use binary.
			triceID = int(binary.BigEndian.Uint16(p.syncBuffer[index+0 : index+1]))

			// next 2 bytes are the count
			count = int(binary.BigEndian.Uint16(p.syncBuffer[index+2 : index+3]))
			//if 0x89ab == triceID && 0xcdef == count {
			//	// remove sync packet
			//	sb0 := p.syncBuffer[:index]
			//	sb1 := p.syncBuffer[index+4:]
			//	p.syncBuffer = append(sb0, sb1...)
			//	triceID = 0
			//	continue
			//}
			//if 0 == triceID { // multi atom trice
			//	index += 4
			//}
			//if 28 < index { // max 32 bytes for one trice, out of sync?
			//	ss[m] = fmt.Sprintln("wrn:ignoring byte", p.syncBuffer[0])
			//	m++
			//	p.syncBuffer = p.syncBuffer[1:] // remove 1st char
			//	index = 0                       // and try again
			//}

			//if 0 == triceID { // p.syncBuffer contains no more full trice.
			//	return
			//}
			trice, ok = p.lut[triceID]
			if !ok { // unknown triceID
				ss[m] = fmt.Sprintln("error:unknown triceID", triceID, "ignoring byte", p.syncBuffer[0])
				m++
				p.syncBuffer = p.syncBuffer[1:] // remove 1st char
				continue
			}

			// check byte count
			var byteCount int
			switch trice.Type {
			case "TRICE0":
				byteCount = 0
			case "TRICE8_1":
				byteCount = 1
			case "TRICE8_2", "TRICE16_1":
				byteCount = 2
			case "TRICE8_3":
				byteCount = 3
			case "TRICE8_4", "TRICE16_2", "TRICE32_1":
				byteCount = 4
			case "TRICE8_5":
				byteCount = 5
			case "TRICE8_6", "TRICE16_3":
				byteCount = 6
			case "TRICE8_7":
				byteCount = 7
			case "TRICE8_8", "TRICE16_4", "TRICE32_2", "TRICE64_1":
				byteCount = 8
			case "TRICE32_3":
				byteCount = 12
			case "TRICE32_4", "TRICE64_2":
				byteCount = 16
			case "TRICE_S":
				byteCount = -1
			default:
				ss[m] = fmt.Sprint("error:unexpected trice.Type", trice.Type, "ignoring byte", p.syncBuffer[0])
				m++
				p.syncBuffer = p.syncBuffer[1:] // remove 1st char
				continue
			}

			if count != byteCount && -1 != byteCount {
				ss[m] = fmt.Sprint("error:byteCount not matching", byteCount, "ignoring byte", p.syncBuffer[0])
				m++
				p.syncBuffer = p.syncBuffer[1:] // remove 1st char
				continue
			}
		}

		// now valid ID and count
		switch trice.Type {
		case "TRICE0":
			ss[m] = fmt.Sprintf(trice.Strg)
			m++
			p.syncBuffer = p.syncBuffer[4:]
			continue
		case "TRICE8_1":
			if 0 != int8(p.syncBuffer[5]) || 0 != int8(p.syncBuffer[6]) || 0 != int8(p.syncBuffer[7]) {
				ss[m] = fmt.Sprint("error:padding bytes not zero, ignoring byte", p.syncBuffer[0])
				m++
				p.syncBuffer = p.syncBuffer[1:] // remove 1st char
				continue
			}
			ss[m] = fmt.Sprintf(trice.Strg, int8(p.syncBuffer[4]))
			m++
			p.syncBuffer = p.syncBuffer[8:]
			continue
		case "TRICE8_2":
			if 0 != int8(p.syncBuffer[6]) || 0 != int8(p.syncBuffer[7]) {
				ss[m] = fmt.Sprint("error:padding bytes not zero, ignoring byte", p.syncBuffer[0])
				m++
				p.syncBuffer = p.syncBuffer[1:] // remove 1st char
				continue
			}
			ss[m] = fmt.Sprintf(trice.Strg,
				int8(p.syncBuffer[4]),
				int8(p.syncBuffer[5]))
			m++
			p.syncBuffer = p.syncBuffer[8:]
			continue
		case "TRICE16_1":
			if 0 != int8(p.syncBuffer[6]) || 0 != int8(p.syncBuffer[7]) {
				ss[m] = fmt.Sprint("error:padding bytes not zero, ignoring byte", p.syncBuffer[0])
				m++
				p.syncBuffer = p.syncBuffer[1:] // remove 1st char
				continue
			}
			ss[m] = fmt.Sprintf(trice.Strg, int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])))
			m++
			p.syncBuffer = p.syncBuffer[8:]
			continue
		case "TRICE8_3":
			if 0 != int8(p.syncBuffer[7]) {
				ss[m] = fmt.Sprint("error:padding bytes not zero, ignoring byte", p.syncBuffer[0])
				m++
				p.syncBuffer = p.syncBuffer[1:] // remove 1st char
				continue
			}
			ss[m] = fmt.Sprintf(trice.Strg,
				int8(p.syncBuffer[4]),
				int8(p.syncBuffer[5]),
				int8(p.syncBuffer[6]))
			m++
			p.syncBuffer = p.syncBuffer[8:]
			continue
		case "TRICE8_4":
			ss[m] = fmt.Sprintf(trice.Strg,
				int8(p.syncBuffer[4]),
				int8(p.syncBuffer[5]),
				int8(p.syncBuffer[6]),
				int8(p.syncBuffer[7]))
			m++
			p.syncBuffer = p.syncBuffer[8:]
			continue
		case "TRICE16_2":
			ss[m] = fmt.Sprintf(trice.Strg,
				int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])),
				int16(binary.BigEndian.Uint16(p.syncBuffer[6:8])))
			m++
			p.syncBuffer = p.syncBuffer[8:]
			continue
		case "TRICE32_1":
			vH := int32(binary.BigEndian.Uint16(p.syncBuffer[4:6])) << 16
			vL := int32(binary.BigEndian.Uint16(p.syncBuffer[6:8]))
			ss[m] = fmt.Sprintf(trice.Strg, vH|vL)
			m++
			p.syncBuffer = p.syncBuffer[8:]
			continue
		case "TRICE8_5":
			if 0 != int8(p.syncBuffer[9]) || 0 != int8(p.syncBuffer[10]) || 0 != int8(p.syncBuffer[11]) {
				ss[m] = fmt.Sprint("error:padding bytes not zero, ignoring byte", p.syncBuffer[0])
				m++
				p.syncBuffer = p.syncBuffer[1:] // remove 1st char
				continue
			}
			ss[m] = fmt.Sprintf(trice.Strg,
				int8(p.syncBuffer[4]),
				int8(p.syncBuffer[5]),
				int8(p.syncBuffer[6]),
				int8(p.syncBuffer[7]),
				int8(p.syncBuffer[8]))
			m++
			p.syncBuffer = p.syncBuffer[12:]
			continue
		case "TRICE8_6":
			if 0 != int8(p.syncBuffer[10]) || 0 != int8(p.syncBuffer[11]) {
				ss[m] = fmt.Sprint("error:padding bytes not zero, ignoring byte", p.syncBuffer[0])
				m++
				p.syncBuffer = p.syncBuffer[1:] // remove 1st char
				continue
			}
			ss[m] = fmt.Sprintf(trice.Strg,
				int8(p.syncBuffer[4]),
				int8(p.syncBuffer[5]),
				int8(p.syncBuffer[6]),
				int8(p.syncBuffer[7]),
				int8(p.syncBuffer[8]),
				int8(p.syncBuffer[9]))
			m++
			p.syncBuffer = p.syncBuffer[12:]
			continue
		case "TRICE16_3":
			if 0 != int8(p.syncBuffer[10]) || 0 != int8(p.syncBuffer[11]) {
				ss[m] = fmt.Sprint("error:padding bytes not zero, ignoring byte", p.syncBuffer[0])
				m++
				p.syncBuffer = p.syncBuffer[1:] // remove 1st char
				continue
			}
			ss[m] = fmt.Sprintf(trice.Strg,
				int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])),
				int16(binary.BigEndian.Uint16(p.syncBuffer[6:8])),
				int16(binary.BigEndian.Uint16(p.syncBuffer[8:10])))
			m++
			p.syncBuffer = p.syncBuffer[12:]
			continue
		case "TRICE8_7":
			if 0 != int8(p.syncBuffer[11]) {
				ss[m] = fmt.Sprint("error:padding bytes not zero, ignoring byte", p.syncBuffer[0])
				m++
				p.syncBuffer = p.syncBuffer[1:] // remove 1st char
				continue
			}
			ss[m] = fmt.Sprintf(trice.Strg,
				int8(p.syncBuffer[4]),
				int8(p.syncBuffer[5]),
				int8(p.syncBuffer[6]),
				int8(p.syncBuffer[7]),
				int8(p.syncBuffer[8]),
				int8(p.syncBuffer[9]),
				int8(p.syncBuffer[10]))
			m++
			p.syncBuffer = p.syncBuffer[12:]
			continue
		case "TRICE8_8":
			ss[m] = fmt.Sprintf(trice.Strg,
				int8(p.syncBuffer[4]),
				int8(p.syncBuffer[5]),
				int8(p.syncBuffer[6]),
				int8(p.syncBuffer[7]),
				int8(p.syncBuffer[8]),
				int8(p.syncBuffer[9]),
				int8(p.syncBuffer[10]),
				int8(p.syncBuffer[11]))
			m++
			p.syncBuffer = p.syncBuffer[12:]
			continue
		case "TRICE16_4":
			ss[m] = fmt.Sprintf(trice.Strg,
				int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])),
				int16(binary.BigEndian.Uint16(p.syncBuffer[6:8])),
				int16(binary.BigEndian.Uint16(p.syncBuffer[8:10])),
				int16(binary.BigEndian.Uint16(p.syncBuffer[10:12])))
			m++
			p.syncBuffer = p.syncBuffer[12:]
			continue
		case "TRICE32_2":
			v0H := int32(binary.BigEndian.Uint16(p.syncBuffer[4:6])) << 16
			v0L := int32(binary.BigEndian.Uint16(p.syncBuffer[6:8]))
			v1H := int32(binary.BigEndian.Uint16(p.syncBuffer[8:10])) << 16
			v1L := int32(binary.BigEndian.Uint16(p.syncBuffer[10:12]))
			ss[m] = fmt.Sprintf(trice.Strg, v0H|v0L, v1H|v1L)
			m++
			p.syncBuffer = p.syncBuffer[12:]
			continue
		case "TRICE64_1":
			vHH := int64(binary.BigEndian.Uint16(p.syncBuffer[4:6])) << 48
			vHL := int64(binary.BigEndian.Uint16(p.syncBuffer[6:8])) << 32
			vLH := int64(binary.BigEndian.Uint16(p.syncBuffer[8:10])) << 16
			vLL := int64(binary.BigEndian.Uint16(p.syncBuffer[10:12]))
			ss[m] = fmt.Sprintf(trice.Strg, vHH|vHL|vLH|vLL)
			m++
			p.syncBuffer = p.syncBuffer[12:]
			continue
		case "TRICE32_3":
			v0H := int32(binary.BigEndian.Uint16(p.syncBuffer[4:6])) << 16
			v0L := int32(binary.BigEndian.Uint16(p.syncBuffer[6:8]))
			v1H := int32(binary.BigEndian.Uint16(p.syncBuffer[8:10])) << 16
			v1L := int32(binary.BigEndian.Uint16(p.syncBuffer[10:12]))
			v2H := int32(binary.BigEndian.Uint16(p.syncBuffer[12:14])) << 16
			v2L := int32(binary.BigEndian.Uint16(p.syncBuffer[14:16]))
			ss[m] = fmt.Sprintf(trice.Strg, v0H|v0L, v1H|v1L, v2H|v2L)
			m++
			p.syncBuffer = p.syncBuffer[16:]
			continue
		case "TRICE32_4":
			v0H := int32(binary.BigEndian.Uint16(p.syncBuffer[4:6])) << 16
			v0L := int32(binary.BigEndian.Uint16(p.syncBuffer[6:8]))
			v1H := int32(binary.BigEndian.Uint16(p.syncBuffer[8:10])) << 16
			v1L := int32(binary.BigEndian.Uint16(p.syncBuffer[10:12]))
			v2H := int32(binary.BigEndian.Uint16(p.syncBuffer[12:14])) << 16
			v2L := int32(binary.BigEndian.Uint16(p.syncBuffer[14:16]))
			v3H := int32(binary.BigEndian.Uint16(p.syncBuffer[16:18])) << 16
			v3L := int32(binary.BigEndian.Uint16(p.syncBuffer[18:20]))
			ss[m] = fmt.Sprintf(trice.Strg, v0H|v0L, v1H|v1L, v2H|v2L, v3H|v3L)
			m++
			p.syncBuffer = p.syncBuffer[20:]
			continue
		case "TRICE64_2":
			v0HH := int64(binary.BigEndian.Uint16(p.syncBuffer[4:6])) << 48
			v0HL := int64(binary.BigEndian.Uint16(p.syncBuffer[6:8])) << 32
			v0LH := int64(binary.BigEndian.Uint16(p.syncBuffer[8:10])) << 16
			v0LL := int64(binary.BigEndian.Uint16(p.syncBuffer[10:12]))
			v1HH := int64(binary.BigEndian.Uint16(p.syncBuffer[12:14])) << 48
			v1HL := int64(binary.BigEndian.Uint16(p.syncBuffer[14:16])) << 32
			v1LH := int64(binary.BigEndian.Uint16(p.syncBuffer[16:18])) << 16
			v1LL := int64(binary.BigEndian.Uint16(p.syncBuffer[18:20]))
			ss[m] = fmt.Sprintf(trice.Strg, v0HH|v0HL|v0LH|v0LL, v1HH|v1HL|v1LH|v1LL)
			m++
			p.syncBuffer = p.syncBuffer[20:]
			continue
		case "TRICE_S":
			ss[m] = fmt.Sprintf(trice.Strg, "to do ...")
			m++
			count += 3
			count &= 4
			p.syncBuffer = p.syncBuffer[4+count:]
			continue

		}
	}
}
