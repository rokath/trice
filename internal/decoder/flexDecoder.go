// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"fmt"
	"io"
	"strings"
	"sync"
	"time"

	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
)

// Flex is the Decoding instance for bare encoded trices.
type Flex struct {
	decoderData
	syncPacket     string
	d0, d1, d2, d3 uint32 // read raw data
	cycle          int
	cycleErrorFlag bool
}

// NewFlexDecoder provides an decoder instance.
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
// littleEndian is false on normal network order.
func NewFlexDecoder(lut id.TriceIDLookUp, m *sync.RWMutex, in io.Reader, endian bool) Decoder {
	p := &Flex{}
	p.in = in
	p.syncBuffer = make([]byte, 0, defaultSize)
	p.lut = lut
	p.lutMutex = m
	p.endian = endian
	p.syncPacket = emitter.SyncPacketPattern
	p.innerReadInterval = 100 * time.Millisecond
	p.cycleErrorFlag = true // avoid cycle error message @ start
	return p
}

// Read is the provided read method for pack decoding of next string as byte slice.
// It uses inner reader p.in and internal id look-up table to fill b with a string.
// b is a slice of bytes with a len for the max expected string size.
// n is the count of read bytes inside b.
// Read returns one trice string (optionally starting wth a channel specifier).
// A line can contain several trice strings.
func (p *Flex) Read(b []byte) (n int, err error) {
	p.b = b
	if time.Since(p.lastInnerRead) > p.innerReadInterval { // poll inner reader
		var m int
		if Verbose { // time measure
			p.lastInnerRead = time.Now()

			// fill intermediate read buffer for pack encoding
			m, err = p.in.Read(b) // use b as intermediate buffer to avoid allocation

			duration := time.Since(p.lastInnerRead).Milliseconds()
			if 0 < duration {
				fmt.Println("Inner Read duration =", duration, "ms.")
			}
		} else { // no time measure
			// fill intermediate read buffer for pack encoding
			m, err = p.in.Read(b) // use b as intermediate buffer to avoid allocation
		}
		// p.syncBuffer can contain unprocessed bytes from last call.
		p.syncBuffer = append(p.syncBuffer, b[:m]...) // merge with leftovers
		if nil != err && io.EOF != err {
			return
		}
	}
	// Even err could be io.EOF some valid data possibly in p.syncBuffer.
	// In case of file input (JLINK usage) a plug off is not detectable here.
	if len(p.syncBuffer) < 4 {
		return // wait
	}
	p.bc = 4 // At least 4 bytes readable. Used by flex decoder only for displaying next few bytes in case out of sync.
	head := p.readU32(p.syncBuffer[0:4])
	if 0x89abcdef == head {
		return p.syncTrice()
	}
	if 0 == 0x80000000&head { // small sub-encoding
		return p.smallSubEncoding(head)
	}
	return p.mediumAndLongSubEncoding(head)
}

func (p *Flex) smallSubEncoding(head uint32) (n int, err error) {
	triceID := id.TriceID(head >> 16) // bits 30...16 are the 15-bit ID
	var ok bool
	p.lutMutex.RLock()
	p.trice, ok = p.lut[triceID] // check lookup table
	p.lutMutex.RUnlock()
	if !ok {
		return p.outOfSync(fmt.Sprintf("unknown triceID %5d", triceID))
	}
	p.d0 = 0xffff & head
	return p.sprintTrice(2)
}

func (p *Flex) mediumAndLongSubEncoding(head uint32) (n int, err error) {
	triceID := id.TriceID(head >> (31 - 20)) // bits 30...11 are the 20-bit ID
	count := int((0x00000f00 & head) >> 8)   // this nibble is the 4-bit count
	cycle := int(0x000000ff & head)          // least significant byte is the cycle
	var cycleWarning string
	if cycle != 0xff&(p.cycle+1) { // lost trices or out of sync
		if !p.cycleErrorFlag {
			cycleWarning = fmt.Sprintln("warning:Cycle", cycle, "does not match expected cyle", p.cycle+1, "- lost trice messages?")
			p.cycleErrorFlag = true
		}
	}

	// 38 13 171 65 239 255 16 0
	if 0xd == count { // TRICE_LONGCOUNT(n), values 0-12 short counts, 0xd is long count and 0xe & 0xf are reserved.
		if len(p.syncBuffer) < 8 {
			return // wait
		}
		countTransfer := p.readU32(p.syncBuffer[4:8])
		count16 := int16(countTransfer >> 16)
		count16invers := int16(countTransfer)
		if count16 != ^count16invers {
			return p.outOfSync(fmt.Sprintf("invalid countTransfer %08x", countTransfer))
		}
		count = int(count16)
	}

	var ok bool
	p.lutMutex.RLock()
	p.trice, ok = p.lut[triceID] // check lookup table
	p.lutMutex.RUnlock()
	if !ok {
		return p.outOfSync(fmt.Sprintf("unknown triceID %5d", triceID))
	}
	p.upperCaseTriceType = strings.ToUpper(p.trice.Type) // for trice* too
	if !p.bytesCountOk(count) {
		return p.outOfSync(fmt.Sprintf("unecpected byteCount, it is not %d", count))
	}
	if !p.isTriceComplete(count) {
		return // try later again
	}
	if !p.readDataAndCheckPaddingBytes(count) {
		return p.outOfSync(fmt.Sprintf("error:padding bytes not zero"))
	}

	// ID and count are ok
	if count > 12 { // remove the 4 long count bytes
		p.syncBuffer = append(p.syncBuffer[0:4], p.syncBuffer[8:]...)
	}
	p.cycleErrorFlag = false
	p.cycle = cycle // Set cycle for checking next trice here because all checks passed.
	p.trice.Strg = cycleWarning + p.trice.Strg
	return p.sprintTrice(count)
}

// readDataAndCheckPaddingBytes checks if existing paddings bytes 0
// after reading data and storing them as 32 bit chunks in p.d0, ... p.d3.
func (p *Flex) readDataAndCheckPaddingBytes(cnt int) (ok bool) {
	b := make([]byte, cnt+3+8) // max 3 more plus head plus possible long count
	copy(b, p.syncBuffer)
	if cnt > 12 {
		b = append(b[0:4], b[8:]...)
	}
	switch p.upperCaseTriceType { // for trice* too {
	case "TRICE0":
		return true
	case "TRICE32_4", "TRICE64_2":
		p.d3 = p.readU32(b[16:20])
		fallthrough
	case "TRICE32_3":
		p.d2 = p.readU32(b[12:16])
		fallthrough
	case "TRICE8_8", "TRICE16_4", "TRICE32_2", "TRICE64_1":
		p.d1 = p.readU32(b[8:12])
		fallthrough
	case "TRICE8_4", "TRICE16_2", "TRICE32_1":
		p.d0 = p.readU32(b[4:8])
		return true // no padding bytes
	case "TRICE_S":
		x := 3 & cnt
		switch x {
		case 0:
			return true // no padding bytes
		case 3:
			ok = 0 == b[4+cnt]
		case 2:
			ok = 0 == b[4+cnt] && 0 == b[1+4+cnt]
		case 1:
			ok = 0 == b[4+cnt] && 0 == b[1+4+cnt] && 0 == b[2+4+cnt]
		}
	default:
		p.d0 = p.readU32(b[4:8])
		switch p.upperCaseTriceType {
		case "TRICE8_1":
			ok = p.d0 < (1 << 8)
		case "TRICE8_2", "TRICE16_1":
			ok = p.d0 < (1 << 16)
		case "TRICE8_3":
			ok = p.d0 < (1 << 24)
		default:
			p.d1 = p.readU32(b[8:12])
			switch p.upperCaseTriceType {
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

// isTriceComplete returns true if triceType payload is complete.
func (p *Flex) isTriceComplete(cnt int) bool {
	longCountBytes := 0
	if cnt > 12 {
		longCountBytes = 4
	}
	cnt += 3
	cnt &= ^3
	if len(p.syncBuffer) < 4+longCountBytes+cnt {
		return false
	}
	return true
}

// bytesCountOk returns true if the transmitted count information matches the expected count.
func (p *Flex) bytesCountOk(cnt int) bool {
	bytesCount := p.expectedByteCount()
	return cnt == bytesCount || -1 == bytesCount
}

// expectedByteCount returns expected byte count for triceType.
// It returns -1 for an unknown count value and -2 for unknown triceType.
func (p *Flex) expectedByteCount() int {
	switch p.upperCaseTriceType {
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
	case "TRICE8_7", "trice8_7":
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

// https://stackoverflow.com/questions/31561369/how-do-i-declare-a-function-pointer-to-a-method-in-go
type flexSelector struct {
	triceType string
	triceFn   func(p *Flex) (int, error)
}

var flexSel = []flexSelector{
	{"TRICE0", (*Flex).trice0},
	{"TRICE8_1", (*Flex).trice81},
	{"Trice8_1", (*Flex).trice81s},
	{"TRICE8_2", (*Flex).trice82},
	{"Trice8_2", (*Flex).trice82s},
	{"TRICE8_3", (*Flex).trice83},
	{"TRICE8_4", (*Flex).trice84},
	{"TRICE8_5", (*Flex).trice85},
	{"TRICE8_6", (*Flex).trice86},
	{"TRICE8_7", (*Flex).trice87},
	{"TRICE8_8", (*Flex).trice88},
	{"TRICE16_1", (*Flex).trice161},
	{"Trice16_1", (*Flex).trice161s},
	{"TRICE16_2", (*Flex).trice162},
	{"TRICE16_3", (*Flex).trice163},
	{"TRICE16_4", (*Flex).trice164},
	{"TRICE32_1", (*Flex).trice321},
	{"TRICE32_2", (*Flex).trice322},
	{"TRICE32_3", (*Flex).trice323},
	{"TRICE32_4", (*Flex).trice324},
	{"TRICE64_1", (*Flex).trice641},
	{"TRICE64_2", (*Flex).trice642},
}

// sprintTrice generates the trice string.
func (p *Flex) sprintTrice(cnt int) (n int, e error) {
	// ID and count are ok
	for _, s := range flexSel {
		if s.triceType == p.upperCaseTriceType {
			return s.triceFn(p)
		}
	}

	if "TRICE_S" == p.upperCaseTriceType {
		return p.triceS(cnt)
	}
	return p.outOfSync(fmt.Sprintf("Unexpected trice.Type %s", p.trice.Type))
}

func (p *Flex) triceS(cnt int) (n int, e error) {
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, string(p.syncBuffer[4:4+cnt])))
	var ct int
	ct = cnt + 3
	ct &= ^3
	p.rubWithLongCount(4+ct, cnt)
	return
}

func (p *Flex) trice0() (n int, e error) {
	n = copy(p.b, fmt.Sprintf(p.trice.Strg))
	p.rub(4)
	return
}

func (p *Flex) trice81s() (n int, e error) {
	d := make([]uint32, 1)
	d[0] = 0xFF & p.d0
	s, b, e := p.uReplace8(d)
	n = copy(p.b, fmt.Sprintf(s, b[0]))
	p.rub(4)
	return
}

func (p *Flex) trice81() (n int, e error) {
	d := make([]uint32, 1)
	d[0] = 0xFF & p.d0
	s, b, e := p.uReplace8(d)
	n = copy(p.b, fmt.Sprintf(s, b[0]))
	p.rub(8)
	return
}

func (p *Flex) trice82s() (n int, e error) {
	d := make([]uint32, 2)
	d[0] = 0xFF & (p.d0 >> 8)
	d[1] = 0xFF & p.d0
	s, b, e := p.uReplace8(d)
	n = copy(p.b, fmt.Sprintf(s, b[0], b[1]))
	p.rub(4)
	return
}

func (p *Flex) trice82() (n int, e error) {
	d := make([]uint32, 2)
	d[0] = 0xFF & (p.d0 >> 8)
	d[1] = 0xFF & p.d0
	s, b, e := p.uReplace8(d)
	n = copy(p.b, fmt.Sprintf(s, b[0], b[1]))
	p.rub(8)
	return
}

func (p *Flex) trice83() (n int, e error) {
	d := make([]uint32, 3)
	d[0] = 0xFF & (p.d0 >> 16)
	d[1] = 0xFF & (p.d0 >> 8)
	d[2] = 0xFF & p.d0
	s, b, e := p.uReplace8(d)
	n = copy(p.b, fmt.Sprintf(s, b[0], b[1], b[2]))
	p.rub(8)
	return
}

func (p *Flex) trice84() (n int, e error) {
	d := make([]uint32, 4)
	d[0] = 0xFF & (p.d0 >> 24)
	d[1] = 0xFF & (p.d0 >> 16)
	d[2] = 0xFF & (p.d0 >> 8)
	d[3] = 0xFF & p.d0
	s, b, e := p.uReplace8(d)
	n = copy(p.b, fmt.Sprintf(s, b[0], b[1], b[2], b[3]))
	p.rub(8)
	return
}

func (p *Flex) trice85() (n int, e error) {
	d := make([]uint32, 5)
	d[0] = 0xFF & (p.d0 >> 24)
	d[1] = 0xFF & (p.d0 >> 16)
	d[2] = 0xFF & (p.d0 >> 8)
	d[3] = 0xFF & p.d0
	d[4] = 0xFF & p.d1
	s, b, e := p.uReplace8(d)
	n = copy(p.b, fmt.Sprintf(s, b[0], b[1], b[2], b[3], b[4]))
	p.rub(12)
	return
}

func (p *Flex) trice86() (n int, e error) {
	d := make([]uint32, 6)
	d[0] = 0xFF & (p.d0 >> 24)
	d[1] = 0xFF & (p.d0 >> 16)
	d[2] = 0xFF & (p.d0 >> 8)
	d[3] = 0xFF & p.d0
	d[4] = 0xFF & (p.d1 >> 8)
	d[5] = 0xFF & p.d1
	s, b, e := p.uReplace8(d)
	n = copy(p.b, fmt.Sprintf(s, b[0], b[1], b[2], b[3], b[4], b[5]))
	p.rub(12)
	return
}

func (p *Flex) trice87() (n int, e error) {
	d := make([]uint32, 7)
	d[0] = 0xFF & (p.d0 >> 24)
	d[1] = 0xFF & (p.d0 >> 16)
	d[2] = 0xFF & (p.d0 >> 8)
	d[3] = 0xFF & p.d0
	d[4] = 0xFF & (p.d1 >> 16)
	d[5] = 0xFF & (p.d1 >> 8)
	d[6] = 0xFF & p.d1
	s, b, e := p.uReplace8(d)
	n = copy(p.b, fmt.Sprintf(s, b[0], b[1], b[2], b[3], b[4], b[5], b[6]))
	p.rub(12)
	return
}

func (p *Flex) trice88() (n int, e error) {
	d := make([]uint32, 8)
	d[0] = 0xFF & (p.d0 >> 24)
	d[1] = 0xFF & (p.d0 >> 16)
	d[2] = 0xFF & (p.d0 >> 8)
	d[3] = 0xFF & p.d0
	d[4] = 0xFF & (p.d1 >> 24)
	d[5] = 0xFF & (p.d1 >> 16)
	d[6] = 0xFF & (p.d1 >> 8)
	d[7] = 0xFF & p.d1
	s, b, e := p.uReplace8(d)
	n = copy(p.b, fmt.Sprintf(s, b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]))
	p.rub(12)
	return
}

func (p *Flex) trice161s() (n int, e error) {
	d := make([]uint32, 1)
	d[0] = 0xFFFF & p.d0
	s, b, e := p.uReplace16(d)
	n = copy(p.b, fmt.Sprintf(s, b[0]))
	p.rub(4)
	return
}

func (p *Flex) trice161() (n int, e error) {
	d := make([]uint32, 1)
	d[0] = 0xFFFF & p.d0
	s, b, e := p.uReplace16(d)
	n = copy(p.b, fmt.Sprintf(s, b[0]))
	p.rub(8)
	return
}

func (p *Flex) trice162() (n int, e error) {
	d := make([]uint32, 2)
	d[0] = 0xFFFF & (p.d0 >> 16)
	d[1] = 0xFFFF & p.d0
	s, b, e := p.uReplace16(d)
	n = copy(p.b, fmt.Sprintf(s, b[0], b[1]))
	p.rub(8)
	return
}

func (p *Flex) trice163() (n int, e error) {
	d := make([]uint32, 3)
	d[0] = 0xFFFF & (p.d0 >> 16)
	d[1] = 0xFFFF & p.d0
	d[2] = 0xFFFF & p.d1
	s, b, e := p.uReplace16(d)
	n = copy(p.b, fmt.Sprintf(s, b[0], b[1], b[2]))
	p.rub(12)
	return
}

func (p *Flex) trice164() (n int, e error) {
	d := make([]uint32, 4)
	d[0] = 0xFFFF & (p.d0 >> 16)
	d[1] = 0xFFFF & p.d0
	d[2] = 0xFFFF & (p.d1 >> 16)
	d[3] = 0xFFFF & p.d1
	s, b, e := p.uReplace16(d)
	n = copy(p.b, fmt.Sprintf(s, b[0], b[1], b[2], b[3]))
	p.rub(12)
	return
}

func (p *Flex) trice321() (n int, e error) {
	d := make([]uint32, 1)
	d[0] = p.d0
	s, b, e := p.uReplace32(d)
	n = copy(p.b, fmt.Sprintf(s, b[0]))
	p.rub(8)
	return
}

func (p *Flex) trice322() (n int, e error) {
	d := make([]uint32, 2)
	d[0] = p.d0
	d[1] = p.d1
	s, b, e := p.uReplace32(d)
	n = copy(p.b, fmt.Sprintf(s, b[0], b[1]))
	p.rub(12)
	return
}

func (p *Flex) trice323() (n int, e error) {
	d := make([]uint32, 3)
	d[0] = p.d0
	d[1] = p.d1
	d[2] = p.d2
	s, b, e := p.uReplace32(d)
	n = copy(p.b, fmt.Sprintf(s, b[0], b[1], b[2]))
	p.rub(16)
	return
}

func (p *Flex) trice324() (n int, e error) {
	d := make([]uint32, 4)
	d[0] = p.d0
	d[1] = p.d1
	d[2] = p.d2
	d[3] = p.d3
	s, b, e := p.uReplace32(d)
	n = copy(p.b, fmt.Sprintf(s, b[0], b[1], b[2], b[3]))
	p.rubWithLongCount(20, 16)
	return
}

func (p *Flex) trice641() (n int, e error) {
	d := make([]uint64, 1)
	d[0] = (uint64(p.d0) << 32) | uint64(p.d1)
	s, b, e := p.uReplace64(d)
	n = copy(p.b, fmt.Sprintf(s, b[0]))
	p.rub(12)
	return
}

func (p *Flex) trice642() (n int, e error) {
	d := make([]uint64, 2)
	d[0] = (uint64(p.d0) << 32) | uint64(p.d1)
	d[1] = (uint64(p.d2) << 32) | uint64(p.d3)
	s, b, e := p.uReplace64(d)
	n = copy(p.b, fmt.Sprintf(s, b[0], b[1]))
	p.rubWithLongCount(20, 16)
	return
}

func (p *Flex) syncTrice() (n int, e error) {
	n = copy(p.b, p.syncPacket)
	p.rub(4)
	return
}

// uReplace8 takes parameter values in d and returns them in b as uint8 or int8 according to %nu occurrences in format string.
// It also returns the modified formatstring with replacments %nu -> %nd.
func (p *Flex) uReplace8(d []uint32) (s string, b []interface{}, e error) {
	b = make([]interface{}, len(d))
	s, u := uReplaceN(p.trice.Strg)
	if len(u) != len(b) {
		e = fmt.Errorf("found %d format specifiers in '%s', expecting %d", len(u), p.trice.Strg, len(b))
		return
	}
	for i := range u {
		if u[i] {
			b[i] = uint8(d[i])
		} else {
			b[i] = int8(d[i])
		}
	}
	return
}

// uReplace16 takes parameter values in d and returns them in b as uint16 or int16 according to %nu occurrences in format string.
// It also returns the modified formatstring with replacments %nu -> %nd.
func (p *Flex) uReplace16(d []uint32) (s string, b []interface{}, e error) {
	b = make([]interface{}, len(d))
	s, u := uReplaceN(p.trice.Strg)
	if len(u) != len(b) {
		e = fmt.Errorf("found %d format specifiers in '%s', expecting %d", len(u), p.trice.Strg, len(b))
		return
	}
	for i := range u {
		if u[i] {
			b[i] = uint16(d[i])
		} else {
			b[i] = int16(d[i])
		}
	}
	return
}

// uReplace32 takes parameter values in d and returns them in b as uint32 or int32 according to %nu occurrences in format string.
// It also returns the modified formatstring with replacments %nu -> %nd.
func (p *Flex) uReplace32(d []uint32) (s string, b []interface{}, e error) {
	b = make([]interface{}, len(d))
	s, u := uReplaceN(p.trice.Strg)
	if len(u) != len(b) {
		e = fmt.Errorf("found %d format specifiers in '%s', expecting %d", len(u), p.trice.Strg, len(b))
		return
	}
	for i := range u {
		if u[i] {
			b[i] = d[i] // is uint32
		} else {
			b[i] = int32(d[i])
		}
	}
	return
}

// uReplace64 takes parameter values in d and returns them in b as uint64 or int64 according to %nu occurrences in format string.
// It also returns the modified formatstring with replacments %nu -> %nd.
func (p *Flex) uReplace64(d []uint64) (s string, b []interface{}, e error) {
	b = make([]interface{}, len(d))
	s, u := uReplaceN(p.trice.Strg)
	if len(u) != len(b) {
		e = fmt.Errorf("found %d format specifiers in '%s', expecting %d", len(u), p.trice.Strg, len(b))
		return
	}
	for i := range u {
		if u[i] {
			b[i] = d[i] // is uint64
		} else {
			b[i] = int64(d[i])
		}
	}
	return
}
