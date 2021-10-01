// Copyright 2020 Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package decoder

import (
	"bytes"
	"encoding/binary"
	"fmt"
	"io"
	"sync"

	"github.com/dim13/cobs"
	"github.com/rokath/trice/internal/id"
)

// COBSR is the Decoding instance for COBSR encoded trices.
type COBSR struct {
	decoderData
	bc int // trice specific bytes count
}

// NewEscDecoder provides an EscDecoder instance.
// l is the trice id list in slice of struct format.
// in is the usable reader for the input bytes.
func NewCOBSRDecoder(lut id.TriceIDLookUp, m *sync.RWMutex, in io.Reader, endian bool) Decoder {
	p := &COBSR{}
	p.in = in
	p.iBuf = make([]byte, 0, defaultSize)
	p.lut = lut
	p.lutMutex = m
	p.endian = endian
	return p
}

// Read is the provided read method for COBSR decoding of next string as byte slice.
// It uses inner reader p.in and internal id look-up table to fill b with a string.
// b is a slice of bytes with a len for the max expected string size.
// n is the count of read bytes inside b.
// Read returns one trice string or nothing.
func (p *COBSR) Read(b []byte) (n int, err error) {
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
	p.iBuf = append(p.iBuf, b[:n]...) // merge with leftovers
	n = 0
	if nil != err && io.EOF != err {
		n = copy(b, fmt.Sprintln("error:internal reader error ", err))
		return
	}

	// Even err could be io.EOF some valid data possibly in p.syncBuffer.
	// In case of file input (J-LINK usage) a plug off is not detectable here.

	// p.bc = len(p.iBuf) // intermediate assignment for better error tracking
	//if p.bc < 4 {
	//	return // wait
	//}
	for {
		index := bytes.IndexByte(p.iBuf, 0)
		if index == -1 {
			return 0, io.EOF
		}

		d := cobs.Decode(p.iBuf[:index+1])

		id := binary.LittleEndian.Uint16(d[1:3])

		fmt.Println(index, p.iBuf[:index+1], " -> ", d, "id = ", id)

		p.iBuf = p.iBuf[index+1:]
	}

	//  if 0xec != p.iBuf[0] { // 0xec == 236
	//  	return p.outOfSync("start byte is not 0xEC")
	//  }
	//  lengthCode := p.iBuf[1]
	//  if 0xde == lengthCode { // 0xde == 222
	//  	return p.outOfSync("0xEC is followed by 0xDE, so no start byte")
	//  }
	//  triceID := id.TriceID(binary.BigEndian.Uint16(p.iBuf[2:4]))
	//  var ok bool
	//  p.lutMutex.RLock()
	//  p.trice, ok = p.lut[triceID]
	//  p.lutMutex.RUnlock()
	//  if !ok { // unknown id
	//  	return p.outOfSync(fmt.Sprint("unknown ID ", triceID))
	//  }
	//  p.upperCaseTriceType = strings.ToUpper(p.trice.Type) // for trice* too
	//  p.bc = p.bytesCount(lengthCode)                      // payload plus header
	//  if p.expectedByteCount() != p.bc {
	//  	return p.outOfSync(fmt.Sprint("trice.Type ", p.trice.Type, " with not matching length code ", lengthCode))
	//  }
	//  if len(p.iBuf) < 4+p.bc { // header plus payload
	//  	return // wait
	//  }
	//  // ID and count are ok
	//  return p.sprintTrice()
}

func (p *COBSR) bytesCount(lc byte) int {
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
func (p *COBSR) expectedByteCount() int {
	switch p.upperCaseTriceType {
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

type COBSRSelector struct {
	triceType string
	triceFn   func(p *COBSR) (int, error)
}

var COBSRSel = []COBSRSelector{
	{"TRICE0", (*COBSR).trice0},
	{"TRICE8_1", (*COBSR).trice81},
	{"TRICE8_2", (*COBSR).trice82},
	{"TRICE8_3", (*COBSR).trice83},
	{"TRICE8_4", (*COBSR).trice84},
	{"TRICE8_5", (*COBSR).trice85},
	{"TRICE8_6", (*COBSR).trice86},
	{"TRICE8_7", (*COBSR).trice87},
	{"TRICE8_8", (*COBSR).trice88},
	{"TRICE16_1", (*COBSR).trice161},
	{"TRICE16_2", (*COBSR).trice162},
	{"TRICE16_3", (*COBSR).trice163},
	{"TRICE16_4", (*COBSR).trice164},
	{"TRICE32_1", (*COBSR).trice321},
	{"TRICE32_2", (*COBSR).trice322},
	{"TRICE32_3", (*COBSR).trice323},
	{"TRICE32_4", (*COBSR).trice324},
	{"TRICE64_1", (*COBSR).trice641},
	{"TRICE64_2", (*COBSR).trice642},
}

func (p *COBSR) sprintTrice() (n int, e error) {
	if "TRICE_S" == p.trice.Type { // special case
		return p.triceS()
	}
	p.rub(4) // remove header

	for _, s := range COBSRSel {
		if s.triceType == p.upperCaseTriceType {
			return s.triceFn(p)
		}
	}
	return p.outOfSync(fmt.Sprintf("Unexpected trice.Type %s", p.trice.Type))
}

func (p *COBSR) triceS() (n int, e error) {
	b := p.iBuf[4:]

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

func (p *COBSR) trice0() (n int, e error) {
	n = copy(p.b, fmt.Sprintf(p.trice.Strg))
	return
}

func (p *COBSR) trice81() (n int, e error) {
	b0 := int8(p.iBuf[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice82() (n int, e error) {
	b0 := int8(p.iBuf[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.iBuf[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice83() (n int, e error) {
	b0 := int8(p.iBuf[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.iBuf[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.iBuf[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.iBuf[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	if 0 != b3 {
		return p.outOfSync("padding byte not zero")
	}
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice84() (n int, e error) {
	b0 := int8(p.iBuf[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.iBuf[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.iBuf[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.iBuf[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice85() (n int, e error) {
	b0 := int8(p.iBuf[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.iBuf[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.iBuf[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.iBuf[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.iBuf[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b5 := int8(p.iBuf[5]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b6 := int8(p.iBuf[6]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b7 := int8(p.iBuf[7]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	if 0 != b7 || 0 != b6 || 0 != b5 {
		return p.outOfSync("padding bytes not zero")
	}
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice86() (n int, e error) {
	b0 := int8(p.iBuf[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.iBuf[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.iBuf[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.iBuf[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.iBuf[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b5 := int8(p.iBuf[5]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b6 := int8(p.iBuf[6]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b7 := int8(p.iBuf[7]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	if 0 != b7 || 0 != b6 {
		return p.outOfSync("padding bytes not zero")
	}
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice87() (n int, e error) {
	b0 := int8(p.iBuf[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.iBuf[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.iBuf[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.iBuf[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.iBuf[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b5 := int8(p.iBuf[5]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b6 := int8(p.iBuf[6]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b7 := int8(p.iBuf[7]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	if 0 != b7 {
		return p.outOfSync("padding byte not zero")
	}
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5, b6))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice88() (n int, e error) {
	b0 := int8(p.iBuf[0]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b1 := int8(p.iBuf[1]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b2 := int8(p.iBuf[2]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b3 := int8(p.iBuf[3]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b4 := int8(p.iBuf[4]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b5 := int8(p.iBuf[5]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b6 := int8(p.iBuf[6]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	b7 := int8(p.iBuf[7]) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, b0, b1, b2, b3, b4, b5, b6, b7))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice161() (n int, e error) {
	d0 := int16(p.readU16(p.iBuf[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice162() (n int, e error) {
	d0 := int16(p.readU16(p.iBuf[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int16(p.readU16(p.iBuf[2:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice163() (n int, e error) {
	d0 := int16(p.readU16(p.iBuf[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int16(p.readU16(p.iBuf[2:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int16(p.readU16(p.iBuf[4:6])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d3 := int16(p.readU16(p.iBuf[6:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	if 0 != d3 {
		return p.outOfSync("padding bytes not zero")
	}
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice164() (n int, e error) {
	d0 := int16(p.readU16(p.iBuf[0:2])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int16(p.readU16(p.iBuf[2:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int16(p.readU16(p.iBuf[4:6])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d3 := int16(p.readU16(p.iBuf[6:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2, d3))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice321() (n int, e error) {
	d0 := int32(p.readU32(p.iBuf[0:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice322() (n int, e error) {
	d0 := int32(p.readU32(p.iBuf[0:4])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int32(p.readU32(p.iBuf[4:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice323() (n int, e error) {
	d0 := int32(p.readU32(p.iBuf[0:4]))   // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int32(p.readU32(p.iBuf[4:8]))   // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int32(p.readU32(p.iBuf[8:12]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d3 := int32(p.readU32(p.iBuf[12:16])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	if 0 != d3 {
		return p.outOfSync("padding bytes not zero")
	}
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice324() (n int, e error) {
	d0 := int32(p.readU32(p.iBuf[0:4]))   // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int32(p.readU32(p.iBuf[4:8]))   // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d2 := int32(p.readU32(p.iBuf[8:12]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d3 := int32(p.readU32(p.iBuf[12:16])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1, d2, d3))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice641() (n int, e error) {
	d0 := int64(p.readU64(p.iBuf[0:8])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0))
	p.rub(p.bc)
	return
}

func (p *COBSR) trice642() (n int, e error) {
	d0 := int64(p.readU64(p.iBuf[0:8]))  // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	d1 := int64(p.readU64(p.iBuf[8:16])) // to do: parse for %nu, exchange with %nd and use than uint8 instead of int8
	n = copy(p.b, fmt.Sprintf(p.trice.Strg, d0, d1))
	p.rub(p.bc)
	return
}
