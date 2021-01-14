// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"fmt"
	"io"

	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/msg"
)

// Pack2 is the Decoder instance for bare encoded trices.
type Pack2 struct {
	decoding
	syncPacket     string
	d0, d1, d2, d3 uint32 // read raw data
	cycle          int
	cycleErrorFlag bool
}

// NewPack2Decoder provides an decoder instance.
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
// littleEndian is false on normal network order.
func NewPack2Decoder(l []id.Item, in io.Reader, endian bool) io.Reader {
	p := &Pack2{}
	p.in = in
	p.syncBuffer = make([]byte, 0, defaultSize)
	p.lut = MakeLut(l)
	p.endian = endian
	p.syncPacket = "inf:[TRICE_SYNCPACKET 0x89abcdef]"
	return p
}

// Read is the provided read method for pack decoding of next string as byte slice.
// It uses inner reader p.in and internal id look-up table to fill b with a string.
// b is a slice of bytes with a len for the max expected string size.
// n is the count of read bytes inside b.
// Read returns one trice string (optionally starting wth a channel specifier).
// A line can contain several trice strings.
func (p *Pack2) Read(b []byte) (n int, err error) {
	sizeMsg := fmt.Sprintln("e:buf too small, expecting", defaultSize, "bytes.")
	if len(b) < len(sizeMsg) {
		return
	}
	if len(b) < defaultSize {
		n = copy(b, sizeMsg)
		return
	}
	p.b = b
	// fill intermediate read buffer for pack encoding
	n, err = p.in.Read(b) // use b as intermediate buffer to avoid allocation
	// p.syncBuffer can contain unprocessed bytes from last call.
	p.syncBuffer = append(p.syncBuffer, b[:n]...) // merge with leftovers
	n = 0
	if nil != err && io.EOF != err {
		return
	}
	// Even err could be io.EOF some valid data possibly in p.syncBuffer.
	// In case of file input (JLINK usage) a plug off is not detectable here.
	if len(p.syncBuffer) < 4 {
		return // wait
	}
	p.bc = 4 // alt least 4 bytes readable

	head := p.readU32(p.syncBuffer[0:4])
	if 0x89abcdef == head {
		return p.syncTrice()
	}
	triceID := int(head >> 16)             // 2 msb bytes are the ID
	count := int((0x0000ff00 & head) >> 8) // next byte is the count
	cycle := int((0x000000ff & head))      // least significant byte is the cycle
	if cycle != p.cycle {                  // lost trices or out of sync
		infoString := fmt.Sprintln("ERROR:received cycle", cycle, " does not match expected cyle", p.cycle)
		msg.Info(infoString)
		p.cycle = cycle
		if false == p.cycleErrorFlag {
			p.cycleErrorFlag = true // assuming lost trices but still in sync
		} else {
			return p.outOfSync(infoString) // out of sync
		}
	} else {
		p.cycleErrorFlag = false
	}
	p.cycle++
	var ok bool
	p.trice, ok = p.lut[triceID] // check lookup table
	if !ok {
		return p.outOfSync(fmt.Sprintf("unknown triceID %5d", triceID))
	}
	if !p.bytesCountOk(count) {
		return p.outOfSync(fmt.Sprintf("unecpected byteCount, it is not %d", count))
	}
	if !p.completeTrice(count) {
		return // try later again
	}
	if !p.readDataAndCheckPaddingBytes(count) {
		return p.outOfSync(fmt.Sprintf("error:padding bytes not zero"))
	}
	// ID and count are ok
	return p.sprintTrice(count)
}

// readDataAndCheckPaddingBytes checks if existing paddings bytes 0
// after reading data and storing them as 32 bit chunks in p.d0, ... p.d3.
func (p *Pack2) readDataAndCheckPaddingBytes(cnt int) (ok bool) {
	switch p.trice.Type {
	case "TRICE0":
		return true
	case "TRICE32_4", "TRICE64_2":
		p.d3 = p.readU32(p.syncBuffer[16:20])
		fallthrough
	case "TRICE32_3":
		p.d2 = p.readU32(p.syncBuffer[12:16])
		fallthrough
	case "TRICE8_8", "TRICE16_4", "TRICE32_2", "TRICE64_1":
		p.d1 = p.readU32(p.syncBuffer[8:12])
		fallthrough
	case "TRICE8_4", "TRICE16_2", "TRICE32_1":
		p.d0 = p.readU32(p.syncBuffer[4:8])
		return true // no padding bytes
	case "TRICE_S":
		x := 3 & cnt
		switch x {
		case 0:
			return true // no padding bytes
		case 3:
			ok = 0 == p.syncBuffer[4+cnt]
		case 2:
			ok = 0 == p.syncBuffer[4+cnt] && 0 == p.syncBuffer[1+4+cnt]
		case 1:
			ok = 0 == p.syncBuffer[4+cnt] && 0 == p.syncBuffer[1+4+cnt] && 0 == p.syncBuffer[2+4+cnt]
		}
	default:
		p.d0 = p.readU32(p.syncBuffer[4:8])
		switch p.trice.Type {
		case "TRICE8_1":
			ok = p.d0 < (1 << 8)
		case "TRICE8_2", "TRICE16_1":
			ok = p.d0 < (1 << 16)
		case "TRICE8_3":
			ok = p.d0 < (1 << 24)
		default:
			p.d1 = p.readU32(p.syncBuffer[8:12])
			switch p.trice.Type {
			case "TRICE8_5":
				ok = p.d1 < (1 << 8)
			case "TRICE8_6", "TRICE16_3":
				ok = p.d1 < (1 << 16)
			case "TRICE8_7":
				ok = p.d1 < (1 << 24)
			}
		}
	}
	if true == ok {
		return
	}
	return false
}

// completeTrice returns true if triceType payload is complete.
func (p *Pack2) completeTrice(cnt int) bool {
	cnt += 3
	cnt &= ^3
	if len(p.syncBuffer) < 4+cnt {
		return false
	}
	return true
}

// bytesCountOk returns true if the transmitted count information matches the expected count.
func (p *Pack2) bytesCountOk(cnt int) bool {
	bytesCount := p.expectedByteCount()
	return cnt == bytesCount || -1 == bytesCount
}

// byteCount returns expected byte count for triceType.
// It returns -1 for an unknown value an -2 for unknown triceType.
func (p *Pack2) expectedByteCount() int {
	switch p.trice.Type {
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

// sprintTrice generates the trice string.
func (p *Pack2) sprintTrice(cnt int) (n int, e error) {
	// ID and count are ok
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
	case "TRICE_S":
		return p.triceS(cnt)
	default:
		return p.outOfSync(fmt.Sprintf("Unexpected trice.Type %s", p.trice.Type))
	}
}

func (p *Pack2) triceS(cnt int) (n int, e error) {
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, string(p.syncBuffer[4:4+cnt])))
	cnt += 3
	cnt &= ^3
	p.rub(4 + cnt)
	return
}

func (p *Pack2) trice0() (n int, e error) {
	n = copy(p.b, fmt.Sprintf(p.trice.Strg))
	p.rub(4)
	return
}

func (p *Pack2) trice81() (n int, e error) {
	b0 := int8(p.d0)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0)) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	p.rub(8)
	return
}

func (p *Pack2) trice82() (n int, e error) {
	b0 := int8(p.d0 >> 8)
	b1 := int8(p.d0)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1))
	p.rub(8)
	return
}

func (p *Pack2) trice83() (n int, e error) {
	b0 := int8(p.d0 >> 16)
	b1 := int8(p.d0 >> 8)
	b2 := int8(p.d0)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2))
	p.rub(8)
	return
}

func (p *Pack2) trice84() (n int, e error) {
	b0 := int8(p.d0 >> 24)
	b1 := int8(p.d0 >> 16)
	b2 := int8(p.d0 >> 8)
	b3 := int8(p.d0)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3))
	p.rub(8)
	return
}

func (p *Pack2) trice85() (n int, e error) {
	b0 := int8(p.d0 >> 24)
	b1 := int8(p.d0 >> 16)
	b2 := int8(p.d0 >> 8)
	b3 := int8(p.d0)
	b4 := int8(p.d1)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4))
	p.rub(12)
	return
}

func (p *Pack2) trice86() (n int, e error) {
	b0 := int8(p.d0 >> 24)
	b1 := int8(p.d0 >> 16)
	b2 := int8(p.d0 >> 8)
	b3 := int8(p.d0)
	b4 := int8(p.d1 >> 8)
	b5 := int8(p.d1)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5))
	p.rub(12)
	return
}

func (p *Pack2) trice87() (n int, e error) {
	b0 := int8(p.d0 >> 24)
	b1 := int8(p.d0 >> 16)
	b2 := int8(p.d0 >> 8)
	b3 := int8(p.d0)
	b4 := int8(p.d1 >> 16)
	b5 := int8(p.d1 >> 8)
	b6 := int8(p.d1)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5, b6))
	p.rub(12)
	return
}

func (p *Pack2) trice88() (n int, e error) {
	b0 := int8(p.d0 >> 24)
	b1 := int8(p.d0 >> 16)
	b2 := int8(p.d0 >> 8)
	b3 := int8(p.d0)
	b4 := int8(p.d1 >> 24)
	b5 := int8(p.d1 >> 16)
	b6 := int8(p.d1 >> 8)
	b7 := int8(p.d1)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5, b6, b7))
	p.rub(12)
	return
}

func (p *Pack2) trice161() (n int, e error) {
	d0 := int16(p.d0)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0))
	p.rub(8)
	return
}

func (p *Pack2) trice162() (n int, e error) {
	d0 := int16(p.d0 >> 16)
	d1 := int16(p.d0)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1))
	p.rub(8)
	return
}

func (p *Pack2) trice163() (n int, e error) {
	d0 := int16(p.d0 >> 16)
	d1 := int16(p.d0)
	d2 := int16(p.d1)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2))
	p.rub(12)
	return
}

func (p *Pack2) trice164() (n int, e error) {
	d0 := int16(p.d0 >> 16)
	d1 := int16(p.d0)
	d2 := int16(p.d1 >> 16)
	d3 := int16(p.d1)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2, d3))
	p.rub(12)
	return
}

func (p *Pack2) trice321() (n int, e error) {
	d0 := int32(p.d0)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0))
	p.rub(8)
	return
}

func (p *Pack2) trice322() (n int, e error) {
	d0 := int32(p.d0)
	d1 := int32(p.d1)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1))
	p.rub(12)
	return
}

func (p *Pack2) trice323() (n int, e error) {
	d0 := int32(p.d0)
	d1 := int32(p.d1)
	d2 := int32(p.d2)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2))
	p.rub(16)
	return
}

func (p *Pack2) trice324() (n int, e error) {
	d0 := int32(p.d0)
	d1 := int32(p.d1)
	d2 := int32(p.d2)
	d3 := int32(p.d3)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2, d3))
	p.rub(20)
	return
}

func (p *Pack2) trice641() (n int, e error) {
	d0 := (int64(p.d0) << 32) | int64(p.d1)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0))
	p.rub(12)
	return
}

func (p *Pack2) trice642() (n int, e error) {
	d0 := (int64(p.d0) << 32) | int64(p.d1)
	d1 := (int64(p.d2) << 32) | int64(p.d3)
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1))
	p.rub(20)
	return
}

func (p *Pack2) syncTrice() (n int, e error) {
	n = copy(p.b, p.syncPacket)
	p.rub(4)
	return
}
