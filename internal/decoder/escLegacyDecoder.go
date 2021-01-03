// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"bytes"
	"encoding/binary"
	"errors"
	"fmt"
	"io"

	"github.com/rokath/trice/internal/id"
)

// Leg is the Decoder instance for esc encoded trices.
type Leg struct {
	decoding
}

// NewEscLegacyDecoder provides an EscDecoder instance.
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
func NewEscLegacyDecoder(l []id.Item, in io.Reader) (p *Leg) {
	p = &Leg{}
	p.in = in
	p.syncBuffer = make([]byte, 0, 2*buffSize)
	p.lut = MakeLut(l)
	return
}

// StringsRead is the provided read method for esc decoding.
// To do: rewrite code
// It uses inner reader and internal id look-up table to fill ss.
// ss is a slice of strings with a len for the max expected strings.
// m is the count of decoded strings inside ss.
func (p *Leg) StringsRead(ss []string) (m int, err error) {
	maxBytes := 4*len(ss) - len(p.syncBuffer) // shortest esc trice is 4 bytes: 0xec 0xdf idHi idLo, so surely read not more than space in ss
	rb := make([]byte, maxBytes)
	var n int
	n, err = p.in.Read(rb)
	p.syncBuffer = append(p.syncBuffer, rb[:n]...) // merge with leftovers
	if nil != err {
		return
	}
	m = -1
	for {
		m++
		if len(p.syncBuffer) < 4 {
			return
		}
		if len(ss) == m {
			err = errors.New("no space in string slice")
			return
		}
		if 0xec != p.syncBuffer[0] { // 0xec == 236
			ss[m] = p.removeFirstByte()
			continue // no start byte
		}
		if 0xde == p.syncBuffer[1] { // 0xde == 222
			ss[m] = p.removeFirstByte()
			m++
			ss[m] = p.removeFirstByte()
			continue // no start byte: `0xec 0xde` is no valid esc packet start. Is is an escaped 0xec.
		}

		// p.syncBuffer[0] is esc, p.syncBuffer[1] is length code, now following the 2 id bytes in bigendian format (network byte order)
		id := (int(p.syncBuffer[2]) << 8) | int(p.syncBuffer[3])
		trice, ok := p.lut[id]
		if !ok { // unknown id
			ss[m] = p.removeFirstByte()
			continue
		}

		switch p.syncBuffer[1] { // length code evaluation
		case 0xdf: // 4 bytes header +  no params
			if "TRICE0" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg)
				p.removeLeadingBytes(4)
				continue
			}
			ss[m] = fmt.Sprintln("error: ", p.syncBuffer)
			p.removeFirstByte()
			continue
		case 0xe0: // 4 bytes header + 1 byte param
			if len(p.syncBuffer) < 5 {
				return // wait
			}
			if "TRICE8_1" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg, int8(p.syncBuffer[4]))
			} else if "TRICE_S" == trice.Type && 0 == int8(p.syncBuffer[4]) {
				ss[m] = fmt.Sprintf(trice.Strg, "")
			} else {
				ss[m] = p.removeFirstByte()
				continue
			}
			p.removeLeadingBytes(5)
			continue
		case 0xe1: // 4 bytes header + 2 bytes param
			if len(p.syncBuffer) < 6 {
				return // wait
			}
			if "TRICE8_2" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]))
			} else if "TRICE16_1" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg, int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])))
			} else if "TRICE_S" == trice.Type && 0 == int8(p.syncBuffer[5]) {
				ss[m] = p.dynString(trice.Strg, 6)
			} else {
				ss[m] = p.removeFirstByte()
				continue
			}
			p.removeLeadingBytes(6)
			continue
		case 0xe2: // 4 bytes header + 4 bytes param or 3 bytes param and 1 byte padding
			if len(p.syncBuffer) < 8 {
				return // wait
			}
			if "TRICE8_3" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]))
			} else if "TRICE8_4" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]),
					int8(p.syncBuffer[7]))
			} else if "TRICE16_2" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg,
					int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[6:8])))
			} else if "TRICE32_1" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg, int32(binary.BigEndian.Uint32(p.syncBuffer[4:8])))
			} else if "TRICE_S" == trice.Type && 0 == int8(p.syncBuffer[7]) { // padding with 0 is expected
				ss[m] = p.dynString(trice.Strg, 8)
			} else {
				ss[m] = p.removeFirstByte()
				continue
			}
			p.removeLeadingBytes(8)
			continue
		case 0xe3: // 4 bytes header + 8 bytes param including 0-3 padding
			if len(p.syncBuffer) < 12 {
				return // wait
			}
			if "TRICE8_5" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]),
					int8(p.syncBuffer[7]),
					int8(p.syncBuffer[8]))
			} else if "TRICE8_6" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]),
					int8(p.syncBuffer[7]),
					int8(p.syncBuffer[8]),
					int8(p.syncBuffer[9]))
			} else if "TRICE16_3" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg,
					int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[6:8])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[8:10])))
			} else if "TRICE8_7" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]),
					int8(p.syncBuffer[7]),
					int8(p.syncBuffer[8]),
					int8(p.syncBuffer[9]),
					int8(p.syncBuffer[10]))
			} else if "TRICE8_8" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg,
					int8(p.syncBuffer[4]),
					int8(p.syncBuffer[5]),
					int8(p.syncBuffer[6]),
					int8(p.syncBuffer[7]),
					int8(p.syncBuffer[8]),
					int8(p.syncBuffer[9]),
					int8(p.syncBuffer[10]),
					int8(p.syncBuffer[11]))
			} else if "TRICE16_4" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg,
					int16(binary.BigEndian.Uint16(p.syncBuffer[4:6])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[6:8])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[8:10])),
					int16(binary.BigEndian.Uint16(p.syncBuffer[10:12])))
			} else if "TRICE32_2" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg,
					int32(binary.BigEndian.Uint32(p.syncBuffer[4:8])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[8:12])))
			} else if "TRICE64_1" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg,
					int64(binary.BigEndian.Uint64(p.syncBuffer[4:12])))
			} else if "TRICE_S" == trice.Type && 0 == int8(p.syncBuffer[11]) { // padding with 0 is expected
				ss[m] = p.dynString(trice.Strg, 12)

			} else {
				ss[m] = p.removeFirstByte()
				continue
			}
			p.removeLeadingBytes(12)
			continue
		case 0xe4: // 4 bytes header + 16 bytes param
			if len(p.syncBuffer) < 20 {
				return // wait
			}
			if "TRICE32_3" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg,
					int32(binary.BigEndian.Uint32(p.syncBuffer[4:8])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[8:12])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[12:16])))
			} else if "TRICE32_4" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg,
					int32(binary.BigEndian.Uint32(p.syncBuffer[4:8])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[8:12])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[12:16])),
					int32(binary.BigEndian.Uint32(p.syncBuffer[16:20])))
			} else if "TRICE64_2" == trice.Type {
				ss[m] = fmt.Sprintf(trice.Strg,
					int64(binary.BigEndian.Uint64(p.syncBuffer[4:12])),
					int64(binary.BigEndian.Uint64(p.syncBuffer[12:20])))
			} else if "TRICE_S" == trice.Type && 0 == int8(p.syncBuffer[19]) { // padding with 0 is expected
				ss[m] = p.dynString(trice.Strg, 20)
			} else {
				ss[m] = p.removeFirstByte()
				continue
			}
			p.removeLeadingBytes(20)
			continue
		case 0xe5: // 4 bytes header + 32 bytes param
			if len(p.syncBuffer) < 36 {
				return // wait
			}
			if "TRICE_S" == trice.Type && 0 == int8(p.syncBuffer[35]) { // padding with 0 is expected
				ss[m] = p.dynString(trice.Strg, 36)
			} else {
				ss[m] = p.removeFirstByte()
				continue
			}
			p.removeLeadingBytes(36)
			continue
		case 0xe6: // 4 bytes header + 64 bytes param
			if len(p.syncBuffer) < 68 {
				return // wait
			}
			if "TRICE_S" == trice.Type && 0 == int8(p.syncBuffer[67]) { // padding with 0 is expected
				ss[m] = p.dynString(trice.Strg, 68)
			} else {
				ss[m] = p.removeFirstByte()
				continue
			}
			p.removeLeadingBytes(68)
			continue
		case 0xe7: // 4 bytes header + 128 bytes param
			if len(p.syncBuffer) < 132 {
				return // wait
			}
			if "TRICE_S" == trice.Type && 0 == int8(p.syncBuffer[131]) { // padding with 0 is expected
				ss[m] = p.dynString(trice.Strg, 132)
			} else {
				ss[m] = p.removeFirstByte()
				continue
			}
			p.removeLeadingBytes(132)
			continue
		case 0xe8: // 4 bytes header + 256 bytes param
			if len(p.syncBuffer) < 260 {
				return // wait
			}
			if "TRICE_S" == trice.Type && 0 == int8(p.syncBuffer[259]) { // padding with 0 is expected
				ss[m] = p.dynString(trice.Strg, 260)
			} else {
				ss[m] = p.removeFirstByte()
				continue
			}
			p.removeLeadingBytes(260)
			continue
		case 0xe9: // 4 bytes header + 512 bytes param
			if len(p.syncBuffer) < 516 {
				return // wait
			}
			if "TRICE_S" == trice.Type && 0 == int8(p.syncBuffer[515]) { // padding with 0 is expected
				ss[m] = p.dynString(trice.Strg, 516)
			} else {
				ss[m] = p.removeFirstByte()
				continue
			}
			p.removeLeadingBytes(516)
			continue
		default:
			ss[m] = p.removeFirstByte()
			continue // invalid length code
		}
	}
}

func (p *Leg) removeLeadingBytes(count int) {
	p.syncBuffer = p.syncBuffer[count:]
}

func (p *Leg) removeFirstByte() string {
	p.removeLeadingBytes(1)
	return fmt.Sprintln("error: ignoring ", p.syncBuffer[0])
}

func (p *Leg) dynString(pFmt string, size int) string {
	byteArray := p.syncBuffer[4:size]
	len := bytes.IndexByte(byteArray, 0)
	z := string(byteArray[:len])
	return fmt.Sprintf(pFmt, z)
}

func (p *Leg) triceS(cnt int) (n int, e error) {
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, string(p.syncBuffer[4:4+cnt])))
	cnt += 3
	cnt &= ^3
	p.rub(4 + cnt)
	return
}
