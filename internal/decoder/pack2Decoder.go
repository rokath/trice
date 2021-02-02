// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"fmt"
	"io"
	"strings"
	"time"

	"github.com/rokath/trice/internal/emitter"
	"github.com/rokath/trice/internal/id"
)

// Pack2 is the Decoding instance for bare encoded trices.
type Pack2 struct {
	decoderData
	syncPacket     string
	d0, d1, d2, d3 uint32 // read raw data
	cycle          int
	cycleErrorFlag bool
}

// NewPack2Decoder provides an decoder instance.
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
// littleEndian is false on normal network order.
func NewPack2Decoder(lut id.TriceIDLookUp, in io.Reader, endian bool) Decoder {
	p := &Pack2{}
	p.in = in
	p.syncBuffer = make([]byte, 0, defaultSize)
	p.lut = lut
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

	if time.Since(p.lastInnerRead) > p.innerReadInterval { // poll inner reader
		if Verbose {
			p.lastInnerRead = time.Now()

			// fill intermediate read buffer for pack encoding
			n, err = p.in.Read(b) // use b as intermediate buffer to avoid allocation
			duration := time.Since(p.lastInnerRead).Milliseconds()
			if 0 < duration {
				fmt.Println("Inner Read duration =", duration, "ms.")
			}
		} else {
			n, err = p.in.Read(b) // use b as intermediate buffer to avoid allocation
		}

		// p.syncBuffer can contain unprocessed bytes from last call.
		p.syncBuffer = append(p.syncBuffer, b[:n]...) // merge with leftovers
		n = 0
		if nil != err && io.EOF != err {
			return
		}
	}
	// Even err could be io.EOF some valid data possibly in p.syncBuffer.
	// In case of file input (JLINK usage) a plug off is not detectable here.
	if len(p.syncBuffer) < 4 {
		return // wait
	}
	p.bc = 4 // At least 4 bytes readable. Used by pack2 decoder only for displaying next few bytes in case out of sync.

	head := p.readU32(p.syncBuffer[0:4])
	if 0x89abcdef == head {
		return p.syncTrice()
	}
	triceID := id.TriceID(head >> (32 - 20)) // 20 most significant bits are the ID
	count := int((0x00000f00 & head) >> 8)   // this nibble is the 4-bit count
	cycle := int((0x000000ff & head))        // least significant byte is the cycle
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
	p.trice, ok = p.lut[triceID] // check lookup table
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
func (p *Pack2) readDataAndCheckPaddingBytes(cnt int) (ok bool) {
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
func (p *Pack2) isTriceComplete(cnt int) bool {
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
func (p *Pack2) bytesCountOk(cnt int) bool {
	bytesCount := p.expectedByteCount()
	return cnt == bytesCount || -1 == bytesCount
}

// expectedByteCount returns expected byte count for triceType.
// It returns -1 for an unknown count value and -2 for unknown triceType.
func (p *Pack2) expectedByteCount() int {
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

// sprintTrice generates the trice string.
func (p *Pack2) sprintTrice(cnt int) (n int, e error) {
	// ID and count are ok
	switch p.upperCaseTriceType {
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
	var ct int
	ct = cnt + 3
	ct &= ^3
	p.rubWithLongCount(4+ct, cnt)
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
	p.rubWithLongCount(20, 16)
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
	p.rubWithLongCount(20, 16)
	return
}

func (p *Pack2) syncTrice() (n int, e error) {
	n = copy(p.b, p.syncPacket)
	p.rub(4)
	return
}
